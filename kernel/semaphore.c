#include "semaphore.h"
#include "defs.h"

struct semaphore semas[MAX_sema];
int idx=0;
uint64 sys_getsema(void){
  int capa;
  if(argint(0,&capa)<0)
    panic("panic sys_getsema");
  semas[idx%MAX_sema].count=capa;
  initlock(&semas[idx].lock, "semalock");  
  return idx++%MAX_sema;
}

uint64 sys_semaP(void){
  int id;
  if(argint(0,&id)<0)
    panic("panic sys_semaP");  
  acquire(&semas[id].lock);
  while(semas[id].count<=0)
    sleep(&semas[id],&semas[id].lock);
  semas[id].count --;
  release(&semas[id].lock);
  return 0;
}

uint64 sys_semaV(void){
  int id;
  if(argint(0,&id)<0)
    panic("panic sys_semaV");
  acquire(&semas[id].lock);
  semas[id].count ++;
  wakeup(&semas[id]);  
  release(&semas[id].lock);
  return 0;
}