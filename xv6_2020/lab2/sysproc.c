#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
  int n;
  struct proc *p = myproc();

  if(argint(0, &n) < 0)
    return -1;

  p->mask_num = n;
  // printf("mask number %d\n",n);
  return 0;
}

uint64
sys_sysinfo(void)
{
  struct proc *p = myproc();


  struct sysinfo info;

  info.nproc = collect_proc();
  info.freemem = collect_freemem();

  // acquire(&p->lock);
  // printf("num of unused proc is %d, memory is %d\n",info.nproc,info.freemem);
  // release(&p->lock);

  uint64 addr; // user pointer to struct sysinfo

  if( argaddr(0, &addr) < 0)
    return -1;

  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
      return -1;
  return 0;



  return 0;
}


// temp
// uint64
// sys_fstat(void)
// {
//   struct file *f;
//   uint64 st; // user pointer to struct stat

//   if(argfd(0, 0, &f) < 0 || argaddr(1, &st) < 0)
//     return -1;
//   return filestat(f, st);
// }


// // Get metadata about file f.
// // addr is a user virtual address, pointing to a struct stat.
// int
// filestat(struct file *f, uint64 addr)
// {
//   struct proc *p = myproc();
//   struct stat st;

//   if(f->type == FD_INODE || f->type == FD_DEVICE){
//     ilock(f->ip);
//     stati(f->ip, &st);
//     iunlock(f->ip);
//     if(copyout(p->pagetable, addr, (char *)&st, sizeof(st)) < 0)
//       return -1;
//     return 0;
//   }
//   return -1;
// }

// // Copy from kernel to user.
// // Copy len bytes from src to virtual address dstva in a given page table.
// // Return 0 on success, -1 on error.
// int
// copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
// {
//   uint64 n, va0, pa0;

//   while(len > 0){
//     va0 = PGROUNDDOWN(dstva);
//     pa0 = walkaddr(pagetable, va0);
//     if(pa0 == 0)
//       return -1;
//     n = PGSIZE - (dstva - va0);
//     if(n > len)
//       n = len;
//     memmove((void *)(pa0 + (dstva - va0)), src, n);

//     len -= n;
//     src += n;
//     dstva = va0 + PGSIZE;
//   }
//   return 0;
// }