#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// prints hello on terminal when called
int sys_hello(void){
  cprintf("hello");
  return 0;
}

// prints information about a process's siblings
int sys_get_sibling_info(void){
  return get_sibling_info();
}

// returns pid of parent
int sys_getppid(void){
  return getppid();
}

// inter-process communication using signals
int sys_signal_process(void){
  int pid;
  int type;
  if (argint(0, &pid) < 0){
    return -1;
  }
  if (argint(1, &type) < 0){
    return -1;
  }
  return signal_process(pid, type);
}

// pause a process for provided duartion
int sys_pause(void){
  int pid;
  int duration;
  if (argint(0, &pid) < 0){
    return -1;
  }
  if (argint(1, &duration) < 0){
    return -1;
  }
  return pause(pid, duration, ticks);
}