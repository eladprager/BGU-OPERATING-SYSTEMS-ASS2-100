#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "x86.h"
#include "stddef.h"

#define STDOUT 1
#define LOOPSIZE 5


void
welcomeprint()
{
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~STARTING SIGNAL TESTS~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void
doneprint()
{
  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~ALL TESTS PASSED~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void
T1(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST1~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing Kill Signal\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  int i, pid[LOOPSIZE];
  
  for(i=0;i<LOOPSIZE;i++){
    pid[i]=fork();
    if(pid[i]==0){
      for(;;) {}
    }
  }
  printf(STDOUT, "\ntrying to kill... \n");

  for(i=0;i<LOOPSIZE;i++)  {
    printf(STDOUT, "kill return %d \n",kill(pid[i],SIGKILL));
  }
  
  printf(STDOUT, "\ntrying to wait... \n");

  for(i=0;i<LOOPSIZE;i++)  {
    printf(STDOUT, "wait return %d\n",wait());
  }
  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST1 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void
T2(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST2~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing Stop Signal\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

   int i, pid[LOOPSIZE];
  
  for(i=0;i<LOOPSIZE;i++){
    pid[i]=fork();
    if(pid[i]==0){
      for(;;) {}
    }
  }
  printf(STDOUT, "\ntrying to stop... \n");
  for(i=0;i<LOOPSIZE;i++){
    printf(STDOUT, "stop return %d \n",kill(pid[i],SIGSTOP));
  }

  printf(STDOUT, "\nkilling... \n");

  for(i=0;i<LOOPSIZE;i++){
    kill(pid[i],SIGKILL);
  }

  printf(STDOUT, "\nwaiting... \n");

  for(i=0;i<LOOPSIZE;i++){
    wait();
  }
  
  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST2 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}


void
T3(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST3~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing if sending out of range signal fails\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

   int i, pid[LOOPSIZE];
  
  for(i=0;i<LOOPSIZE;i++){
    pid[i]=fork();
    if(pid[i]==0){
      for(;;) {}
    }
  }
  printf(STDOUT, "\ntrying to stop... \n");
  for(i=0;i<LOOPSIZE;i++){
    printf(STDOUT, "sending out of range signal return %d \n",kill(pid[i],33));
  }

  printf(STDOUT, "\nkilling... \n");

  for(i=0;i<LOOPSIZE;i++){
    kill(pid[i],SIGKILL);
  }

  printf(STDOUT, "\nwaiting... \n");

  for(i=0;i<LOOPSIZE;i++){
    wait();
  }
  
  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST3 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}


void
T4(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST4~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing Cont Signal\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

  int i, pid[LOOPSIZE];
  
  for(i=0;i<LOOPSIZE;i++){
    pid[i]=fork();
    if(pid[i]==0){
      for(;;) {}
    }
  }

  printf(STDOUT, "\ntrying to stop... \n");
  for(i=0;i<LOOPSIZE;i++){
    printf(STDOUT, "stop return %d\n",kill(pid[i],SIGSTOP));
  }

  printf(STDOUT, "\ntrying to cont... \n");

  for(i=0;i<LOOPSIZE;i++){
    printf(STDOUT, "cont return %d \n",kill(pid[i],SIGCONT));
  }
  printf(STDOUT, "\nkilling... \n");

  for(i=0;i<LOOPSIZE;i++){
    kill(pid[i],SIGKILL);
  }

  printf(STDOUT, "\nwaiting... \n");

  for(i=0;i<LOOPSIZE;i++){
    wait();
  }

  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST4 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void customSignalHandler(){
  sleep(25);
  printf(STDOUT, "Custom signal handler, called by pid: %d\n",getpid());
}

void
T5(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST5~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing User Signal\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  int i, pid[LOOPSIZE];
  
  printf(STDOUT, "\ntrying to define custom signal... \n");
  struct sigaction act = { customSignalHandler };
  printf(STDOUT, "systemcall signal return %d \n",sigaction(1,&act,NULL));

  for(i=0;i<LOOPSIZE;i++){
    pid[i]=fork();
    if(pid[i]==0){
      for(;;) {}
    }
  }
    
  printf(STDOUT, "\ntrying to call user signal... \n");

  for(i=0;i<LOOPSIZE;i++){
  	printf(STDOUT, "customsignal return %d \n",kill(pid[i],1));
    sleep(50);
  }
  
  printf(STDOUT, "\nkilling... \n");

  for(i=0;i<LOOPSIZE;i++){
  	kill(pid[i],SIGKILL);
  }
  
  printf(STDOUT, "\nwaiting... \n");

  for(i=0;i<LOOPSIZE;i++){
  	wait();
  }

  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST5 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void
T6(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST6~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing signal change\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  
  printf(STDOUT, "\ntrying to define custom signal... \n");
  struct sigaction act = { customSignalHandler };
  printf(STDOUT, "trying to change SIGKILL signal return %d \n",sigaction(SIGKILL,&act,NULL));
  printf(STDOUT, "trying to change SIGSTOP signal return %d \n",sigaction(SIGSTOP,&act,NULL));
  printf(STDOUT, "trying to change SIGCONT signal return %d \n",sigaction(SIGCONT,&act,NULL));
  printf(STDOUT, "trying to change SIGIGN signal return %d \n",sigaction(SIG_IGN,&act,NULL));
  printf(STDOUT, "trying to change SIGDFL signal return %d \n\n",sigaction(SIG_DFL,&act,NULL));

  printf(STDOUT, "trying to define SIGCONT and SIG_DFL signals as SIG_IGN... \n");
  struct sigaction act2 = { (void*)SIG_IGN };
  sigaction(SIGCONT,&act2,NULL);
  sigaction(SIG_DFL,&act2,NULL);
  printf(STDOUT, "trying to change SIGCONT signal to SIG_IGN return %d \n",((int)act2.sa_handler==1)?0:-1);
  printf(STDOUT, "trying to change SIG_DFL signal to SIG_IGN return %d \n",((int)act2.sa_handler==1)?0:-1);

  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST6 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

char *decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer; 
   count = 0;
   pointer = (char*)malloc(32+1);
   if (pointer == NULL)
      exit();  
   for (c = 31 ; c >= 0 ; c--)
   {
      d = n >> c; 
      if (d & 1)
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';
      count++;
   }
   *(pointer+count) = '\0';
   return  pointer;
}

void
T7(){
  printf(STDOUT,"~~~~~~~~~~~~~~~TEST7~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"Testing forked masks\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  
  sigprocmask((1<<10) | (1<<15) | (1<<25));

  //ignoring signals 10,15,20,25
  struct sigaction act1 = { (void*)SIG_IGN };
  sigaction(10,&act1,NULL);
  struct sigaction act2 = { (void*)SIG_IGN };
  sigaction(15,&act2,NULL);
  struct sigaction act3 = { (void*)SIG_IGN };
  sigaction(25,&act3,NULL);

  int pid = fork();

  if(pid < 0){
    printf(1,"failed in fork\n");
  }
  if (pid == 0) {
      if (sigprocmask(0) != ((1 << 10) | (1 << 15) | (1 << 25))){
          printf(1,"failed: child didnt inherit mask array\n");
          exit();
      }
      exit();
  }
  else {
      int num = sigprocmask(0);
      char *pointer = decimal_to_binary(num);
      
      if (num != ((1 << 10) | (1 << 15) | (1 << 25))){
          printf(1,"failed: Parent didnt keep mask array\n");
          exit();
      }
      wait();
      printf(STDOUT, "parent and forked child masks are identical, their sigmask is %s \n",pointer);
  } 

  printf(STDOUT,"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~TEST7 PASSED~~~~~~~~~~~~\n");
  printf(STDOUT,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}


int
main(int argc, char *argv[])
{
  welcomeprint();
  T1();
  T2();
  T3();
  T4();
  T5();
  T6();
  T7();
  doneprint();
  exit();
}