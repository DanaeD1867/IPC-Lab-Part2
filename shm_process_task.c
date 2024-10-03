#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <time.h>

void ChildProcess(int []); 
void ParentProcess(int []); 

int main(int  argc, char *argv[]){
  int shmID; 
  int *shmPTR; 
  pid_t pid;
  int status; 

  shmID = shmget(IPC_PRIVATE, 3* sizeof(int), IPC_CREAT | 0666);

  if(shmID < 0){
    printf("***shmget error (server) *** \n"); 
    exit(1); 
  } 

  printf("Server has recieved a shared memory of two integers. \n"); 

  shmPTR = (int *)shmat(shmID, NULL, 0); 
  if(*shmPTR == -1){
    printf("*** shmat error (server) ***\n"); 
    exit(1); 
  }

  printf("Server has attached the shared memory. \n");

  shmPTR[0] = 0; //BankAccount
  shmPTR[1] = 0; //Turn
  shmPTR[2] = 0; //Counter
 
  
  printf("Server is about to fork a child process. \n");
  pid = fork(); 
  if(pid < 0){
    printf("*** for error (server) ***\n"); 
    exit(1); 
  }else if(pid == 0){
    ChildProcess(shmPTR);
    exit(0);  
  }else{
    ParentProcess(shmPTR); 
  }

  wait(&status); 
  printf("Child process has completed \n"); 
  shmdt((void *)shmPTR); 
  printf("Server has detached its shared memory \n"); 
  shmctl(shmID, IPC_RMID, NULL); 
  printf("Server has removed its shared memory\n"); 
  printf("Server exits...\n"); 

  return 0; 
}

void ChildProcess(int shmPTR[]){
  srand(time(0) + 1); 

  while(shmPTR[2] < 25){
     
    sleep(rand() % 6); 

   
      int account = shmPTR[0];
      int withdrawal = rand() % 51; 
      printf("Poor student needs $%d\n", withdrawal); 

       while(shmPTR[1] != 1){
      //Wait
      }

      if(withdrawal <= account){
        account -= withdrawal; 
        printf("Student: Withdraws: $%d, Balance = $%d\n", withdrawal, account); 
      }else{
        printf("Student: Not enough cash\n"); 
      }

      shmPTR[0] = account; 
      shmPTR[1] = 0; 
      shmPTR[2] += 1;
  } 
}

void ParentProcess(int shmPTR[]){
  srand(time(0)); 

   
  while(shmPTR[2] < 25){

    sleep(rand() % 6);
    int account = shmPTR[0];

    while(shmPTR[1] != 0){
      //Wait
    }
      if(account <= 100){
        int deposit = rand() % 101; 
        if(deposit % 2 == 0){
          account += deposit; 
          printf("Dad: Deposits: $%d, Balance = $%d\n", deposit, account); 
        } else{
          printf("Dad doesn't have any money to give\n"); 
        }
        }else{
          printf("Dad: Thinks student has enough cash\n"); 
        }
        shmPTR[0] = account; 
        shmPTR[1] = 1; 
        shmPTR[2] += 1; 
  }
}