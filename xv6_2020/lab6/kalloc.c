// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  int   ref_cnt[32*1024];
} cow;

void increase_cow_ref_num(uint64 pa){
  acquire(&cow.lock);
  uint64 pa_index = ((uint64)pa & 0x7ffffff) / PGSIZE;
  cow.ref_cnt[pa_index] += 1;
  release(&cow.lock);
}




void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);

  initlock(&cow.lock, "cow");
  acquire(&cow.lock);
  for(int i=0;i<32*1024;i++){
    cow.ref_cnt[i] = 0;
  }
  release(&cow.lock);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");


  acquire(&kmem.lock);
  acquire(&cow.lock);
  uint64 pa_index = ((uint64)pa & 0x7ffffff) / PGSIZE;
  if(cow.ref_cnt[pa_index] > 1){
    cow.ref_cnt[pa_index] -= 1;
    release(&cow.lock);
    release(&kmem.lock);
    return;
  }
  cow.ref_cnt[pa_index] = 0;
  release(&cow.lock);
  release(&kmem.lock);



  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r){
    memset((char*)r, 5, PGSIZE); // fill with junk

    acquire(&cow.lock);
    uint64 pa_index = ((uint64)r & 0x7ffffff) / PGSIZE;
    cow.ref_cnt[pa_index] = 1;
    release(&cow.lock);
  }

  return (void*)r;
}
