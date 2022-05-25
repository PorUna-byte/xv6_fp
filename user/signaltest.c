#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"
#include "kernel/signal.h"

void
usr_handler0()
{
  printf("usr_handler0!\n");
  sigreturn();
}
void
usr_handler1()
{
  printf("usr_handler1!\n");
  sigreturn();
}

void
usr_handler2()
{
  printf("usr_handler2!\n");
  sigreturn();
}

void
usr_handler3()
{
  printf("usr_handler3!\n");
  sigreturn();
}
void
usr_handler4()
{
  printf("usr_handler4!\n");
  sigreturn();
}
void
usr_handler5()
{
  printf("usr_handler5!\n");
  sigreturn();
}
void
usr_handler6()
{
  printf("usr_handler6!\n");
  sigreturn();
}
void
usr_handler7()
{
  printf("usr_handler7!\n");
  sigreturn();
}
void
usr_handler8()
{
  printf("usr_handler8!\n");
  sigreturn();
}
void
usr_handler9()
{
  printf("usr_handler9!\n");
  sigreturn();
}
void test1()
{
  int pids[10];
  handler_type usr_handlers[10]={usr_handler0,usr_handler1,usr_handler2,
  usr_handler3,usr_handler4,usr_handler5,
  usr_handler6,usr_handler7,usr_handler8,usr_handler9};
  for(int i=0;i<10;i++){
    if((pids[i]=fork())==0){
      bindsig(usr_handlers[i],i);
      sleep(10*10000);
      exit(0);
    }
  }
  sleep(5);
  for(int i=0;i<10;i++)
    sendsig(pids[i],i);
  for(int i=0;i<10;i++)  
    wait(0);
  exit(0);
}
void test2()
{
  int pid;
  if((pid=fork())==0){
    bindsig(usr_handler1,1);
    bindsig(usr_handler2,2);
    bindsig(usr_handler3,3);
    sleep(10*10000);
    sleep(10*10000);
    sleep(10*10000);
    exit(0);
  }else
  {
    sleep(5);
    sendsig(pid,1);
    sleep(5);
    sendsig(pid,2);
    sleep(5);
    sendsig(pid,3); 
    wait(&pid);       
  } 
  exit(0); 
}
void test3()
{
  bindsig(usr_handler5,5);
  for(int i=0;i<(1<<28);i++)
    if(i%(int)1e5==0)
      printf("%d\n",i);
  return ;    
}
void test4()
{
  int id=getsema(0);  //acquire a semaphore with capacity:10
  int p[2];
  pipe(p);
  int token=0;
  int child_num=0;
  int child_pids[10];
  for(int i=0;i<100;i++)
    write(p[1],&token,sizeof(token));  //generate a lot of trash

  token=1; 
  printf("---begin test---\n"); 
  while(child_num<10&&(child_pids[child_num]=fork())!=0) child_num++; 

  if(child_num==10){
    //parent goes here
    int trash;
    sleep(20);
    for(int i=0;i<100;i++)
      read(p[0],&trash,sizeof(trash));  //consume all trash
    //parent process generates token here
    while(token<=1000){
      write(p[1],&token,sizeof(token)); 
      token++;
      semaV(id); 
    }
    sleep(100);
    printf("---finish test---\n");
    for(child_num=0;child_num<10;child_num++)
      kill(child_pids[child_num]);
    for(child_num=0;child_num<10;child_num++){      
      semaV(id); //prevent the child from being truck in the semaP routine
      printf("wait pid:%d\n",wait(0));  
    }  
    return ;
  }
  //child process goes here
  while(1){
    semaP(id);
    read(p[0],&token,sizeof(token));
    printf("hello from pid:%d with token:%d\n",getpid(),token);
    sleep(1);
  }
  return ;      
}
int
main(int argc, char *argv[])
{
  test4();
  exit(0);
}
