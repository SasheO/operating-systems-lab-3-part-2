#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ChildProcess(int []);
void  ParentProcess(int []);

int  main(int  argc, char *argv[]){
  int    BankAcctID;
  int    *BankAcctPtr;
  int    TurnID;
  int    *TurnPtr;
  pid_t  pid;
  int    status;

  // provision shared memory for BankAcctID and TurnID, see any errors in allocating shared memory
  BankAcctID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  if (BankAcctID < 0) {
      printf("*** shmget error ***\n");
      exit(1);
  }
  TurnID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  if (TurnID < 0) {
      printf("*** shmget error ***\n");
      exit(1);
  }
  printf("Server has received a shared memory of four integers...\n");

  // set BankAcctPtr and TurnPtr pointers to the shared memory provisioned
  BankAcctPtr = (int *) shmat(BankAcctID, NULL, 0);
  if (*BankAcctPtr == -1) {
      printf("*** shmat error ***\n");
      exit(1);
  }
  TurnPtr = (int *) shmat(TurnID, NULL, 0);
  if (*TurnPtr == -1) {
      printf("*** shmat error ***\n");
      exit(1);
  }

  *BankAcctPtr = 0; // initialize value to 0
  *TurnPtr = 0; // initialize value to 0


  pid = fork();
  if (pid < 0) {
      printf("*** fork error ***\n");
      exit(1);
  }
  else if (pid == 0) {
      ChildProcess(BankAcctPtr);
      // exit(0);
  }
  else{
    ParentProcess(BankAcctPtr);
  }

  return 0;
}

void  ChildProcess(int  SharedMem[]){}
void  ParentProcess(int  SharedMem[]){
  int sleep_time;
  int account; // local variable for bank accound amount
  sleep_time = random()%5+1;
  sleep(sleep_time);


}