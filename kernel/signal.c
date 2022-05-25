#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "signal.h"

extern struct proc proc[NPROC];
uint64
sys_sendsig(void)
{
  struct proc *p;
  int pid;
  int signo;
  if(argint(0,&pid)<0)
    panic("sys_sendsig");
  if(argint(1,&signo)<0)
    panic("sys_sendsig");
  for(p=proc;p<&proc[NPROC];p++){
    acquire(&p->lock);
    if(p->state!=UNUSED&&p->state!=ZOMBIE&&p->pid==pid){
      p->signals |= ((unsigned int)1<<signo);
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }
  return -1;  
}
uint64
sys_bindsig(void)
{ 
  struct proc *p=myproc();
  uint64 handler;
  int signo;
  if(argaddr(0,&handler)<0)
    panic("sys_bindsig");
  if(argint(1,&signo)<0)
    panic("sys_bindsig");
  p->handlers[signo]=(handler_type)handler;
  return 0;
}
uint64 
sys_sigalarm(void)
{
  struct proc *p=myproc();
  int interval;
  uint64 handler;
  if(argint(0,&interval)<0)
    panic("sigalarm can't acquire time interval");
  if(argaddr(1,&handler)<0)  
    panic("sigalarm can't acquire handler");
  p->interval=interval;
  p->ticks_left=interval;
  p->handlers[SIG_ALARM]=(handler_type)handler;
  return 0;
}
uint64 
sys_sigreturn(void)
{
  struct proc* p=myproc();
  *p->trapframe=*p->saved_trapframe;
  p->re_entrant=0;
  return 0;
}