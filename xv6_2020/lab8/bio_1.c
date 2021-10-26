// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.
/*
 * 没有实现 LRU，看完文件系统在回来做
 *
 * */

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

#define NBUCKET 13
struct {

  struct spinlock bget_steal_lock;
  struct spinlock lock[NBUCKET];
  struct buf buf[NBUF];

  // Linked list of all buffers, through prev/next.
  // Sorted by how recently the buffer was used.
  // head.next is most recent, head.prev is least.
  struct buf head[NBUCKET];
} bcache;

int get_hash_num(int num){
  return num % NBUCKET;
}

void
binit(void)
{
  struct buf *b;
  initlock(&bcache.bget_steal_lock, "bcache.bucket");

  for (int i = 0; i < NBUCKET; i++)
  {
    initlock(&bcache.lock[i], "bcache.bucket");
    bcache.head[i].prev = &bcache.head[i];
    bcache.head[i].next = &bcache.head[i];
  }


 for (b = bcache.buf; b < bcache.buf + NBUF; b++)
  {
    // int hash = get_hash_num(b->blockno);
    b->next = bcache.head[0].next;
    b->prev = &bcache.head[0];
    initsleeplock(&b->lock, "buffer");
    bcache.head[0].next->prev = b;
    bcache.head[0].next = b;
  }
  // for (int i = 0;i<NBUF;i++)
  // {
  //   int hash = get_hash_num(i);
  //   b = &bcache.buf[i];
  //   b->timestamp = ticks;
  //   b->next = bcache.head[hash].next;
  //   b->prev = &bcache.head[hash];
  //   initsleeplock(&b->lock, "buffer");
  //   bcache.head[hash].next->prev = b;
  //   bcache.head[hash].next = b;
  // }
}


// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;
  int hash = get_hash_num(blockno);

  acquire(&bcache.lock[hash]);

  // Is the block already cached?
  for(b = bcache.head[hash].next; b != &bcache.head[hash]; b = b->next){
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      b->ticks = ticks;
      release(&bcache.lock[hash]);
      acquiresleep(&b->lock);
      return b;
    }
  }
  release(&bcache.lock[hash]);

  acquire(&bcache.bget_steal_lock);
  acquire(&bcache.lock[hash]);

  for(b = bcache.head[hash].next; b != &bcache.head[hash]; b = b->next){
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      b->ticks = ticks;
      release(&bcache.lock[hash]);
       release(&bcache.bget_steal_lock);
      acquiresleep(&b->lock);
      return b;
    }
  }

  // Not cached.
  // TODO Recycle the least recently used (LRU) unused buffer.
  for(int i=0;i<NBUCKET;i++){
    if(i != hash){
      acquire(&bcache.lock[i]);
      for(b = bcache.head[i].next; b != &bcache.head[i]; b = b->next){
        if(b->refcnt == 0) {
          b->dev = dev;
          b->blockno = blockno;
          b->valid = 0;
          b->refcnt = 1;
          b->ticks = ticks;

          b->next->prev = b->prev;
          b->prev->next = b->next;
          release(&bcache.lock[i]);

          b->next = bcache.head[hash].next;
          b->prev = &bcache.head[hash];
          bcache.head[hash].next->prev = b;
          bcache.head[hash].next = b;

          release(&bcache.lock[hash]);
          release(&bcache.bget_steal_lock);
          acquiresleep(&b->lock);
          return b;
        }
      }
      release(&bcache.lock[i]);
    }
  }
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");

  releasesleep(&b->lock);
  int hash = get_hash_num(b->blockno);

  acquire(&bcache.lock[hash]);
  b->refcnt--;
  if (b->refcnt == 0) {
    // no one is waiting for it.
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = bcache.head[hash].next;
    b->prev = &bcache.head[hash];
    bcache.head[hash].next->prev = b;
    bcache.head[hash].next = b;
  }

  release(&bcache.lock[hash]);
}

void
bpin(struct buf *b) {
  int hash = get_hash_num(b->blockno);

  acquire(&bcache.lock[hash]);
  b->refcnt++;
  release(&bcache.lock[hash]);
}

void
bunpin(struct buf *b) {
  int hash = get_hash_num(b->blockno);
  acquire(&bcache.lock[hash]);
  b->refcnt--;
  release(&bcache.lock[hash]);
}


