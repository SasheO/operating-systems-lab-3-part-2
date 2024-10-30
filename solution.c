#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

void ChildProcess(int*, int*);
void  ParentProcess(int*, int*);
void  DepositMoney();

int  main(int  argc, char *argv[]){
  int    BankAcctID;
  int    *BankAcctPtr;
  int    TurnID;
  int    *TurnPtr;
  pid_t  pid;

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
      ChildProcess(BankAcctPtr, TurnPtr);
      // exit(0);
  }
  else{
    ParentProcess(BankAcctPtr, TurnPtr);
  }

  return 0;
}


void  ChildProcess(int  *BankAcctPtr, int *TurnPtr){
  srandom(time(NULL));
  int sleep_time, account, balance, indx;
  for (indx=0; indx < 25; indx ++){
    sleep_time = random()%5+1;
    sleep(sleep_time);
    account = *BankAcctPtr; // copy the in BankAccount to a local variable account
    while(*TurnPtr!=1);
    balance = random()%50;
    printf("Poor Student needs $%d\n", balance);
    if (balance<=account){
      account -= balance;
      printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
    }
    else{
      printf("Poor Student: Not Enough Cash ($%d)\n", account );
    }
    * BankAcctPtr = account;
    * TurnPtr = 0;
  }
}

void  ParentProcess(int * BankAcctPtr, int *TurnPtr){
  srandom(time(NULL));
  int sleep_time, indx;
  int account; // local variable for bank account amount
  for (indx=0; indx < 25; indx ++){
    int * accountPTR = &account;
    sleep_time = random()%5+1;
    sleep(sleep_time);
    account = *BankAcctPtr; // copy the in BankAccount to a local variable account
    while(*TurnPtr!=0);
    if (account <= 100){
      DepositMoney(accountPTR, TurnPtr);
      *BankAcctPtr = *accountPTR;
    }
    else{
      printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
    }
  }
}

void DepositMoney(int *accountPTR, int *TurnPtr){
  srandom(time(NULL));
  int deposit_amount = random()%100;
  // * accountPTR = 100;
  if (deposit_amount%2==0){
    // TODO: deposit amount into account
    * accountPTR += deposit_amount;
    printf("Dear old Dad: Deposits $%d / Balance = $%d\n", deposit_amount, *accountPTR);
  }
  else{
    printf("Dear old Dad: Doesn't have any money to give\n");
  }
  *TurnPtr = 1;

}