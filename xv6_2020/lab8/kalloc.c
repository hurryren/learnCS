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

struct kmem{
  struct spinlock lock;
  struct run *freelist;
};

struct kmem mem_cpu[NCPU];

// ----------------------- orange add -----------------------
static void mul_kinit(){
  for(int i = 0;i<NCPU;i++){
    initlock(&mem_cpu[i].lock, "kmem");
  }
}
static int get_next_cpu(int num){
  if(num == (NCPU-1)){
    return 0;
  }
  else{
    return num + 1;
  }
}


static void mul_kfree(void *pa,struct kmem *mem){
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&mem->lock);
  r->next = mem->freelist;
  mem->freelist = r;
  release(&mem->lock);
}

static void * mul_kalloc(int cpu_id)
{

  struct run *r;
  acquire(&mem_cpu[cpu_id].lock);
  r = mem_cpu[cpu_id].freelist;
  if(r)
    mem_cpu[cpu_id].freelist = r->next;
  release(&mem_cpu[cpu_id].lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

static void mul_freerange(void *pa_start, void *pa_end){
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  int cpu_num = 0;
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE){
    mul_kfree(p,&mem_cpu[cpu_num]);
    cpu_num = get_next_cpu(cpu_num);
  }
}


// ----------------------- orange add -----------------------


void
kinit()
{
  // initlock(&kmem.lock, "kmem");
  // freerange(end, (void*)PHYSTOP);
  mul_kinit();
  mul_freerange(end, (void*)PHYSTOP);
}

// void
// freerange(void *pa_start, void *pa_end)
// {
//   char *p;
//   p = (char*)PGROUNDUP((uint64)pa_start);
//   for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
//     kfree(p);
// }

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  push_off();
  int cpu_id = cpuid();
  pop_off();
  mul_kfree(pa,&mem_cpu[cpu_id]);


  // struct run *r;

  // if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
  //   panic("kfree");

  // // Fill with junk to catch dangling refs.
  // memset(pa, 1, PGSIZE);

  // r = (struct run*)pa;

  // acquire(&kmem.lock);
  // r->next = kmem.freelist;
  // kmem.freelist = r;
  // release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  push_off();
  int cpu_id = cpuid();
  pop_off();

  struct run *r;
  int return_id=1;
  for(int i = cpu_id;i!=cpu_id || return_id;i=get_next_cpu(i)){
    if(i == cpu_id){
      return_id = 0;
    }
    r = (struct run*)mul_kalloc(i);
    if(r){
      memset((char*)r, 5, PGSIZE); // fill with junk
      return (void*)r;
    }
  }



  // acquire(&kmem.lock);
  // r = kmem.freelist;
  // if(r)
  //   kmem.freelist = r->next;
  // release(&kmem.lock);

  // if(r)
  //   memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
