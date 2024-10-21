#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int n=9;
int fibonacciSequence(int fib_n_1, int fib_n_2){
  return fib_n_1+fib_n_2;
}

int main(int argc, char **argv)
{
  int segment_id=shmget(IPC_PRIVATE, 10*sizeof(char),S_IRUSR|S_IWUSR);
  char *sharedMemory;
  printf("Sou o processo pai, vou criar um filho para calcular a sequência de Fibonacci com %d termos.\n", n);
  fflush(stdout);
  pid_t pid=fork();
  if(pid==0){
    sharedMemory=(char *) shmat(segment_id, NULL,0);
    
    int numbersArray[n+1]; 
    numbersArray[0]=0; 
    sprintf(sharedMemory, "%d ", numbersArray[0]);
    if(n>0){
      numbersArray[1]=1;
      sprintf(sharedMemory + strlen(sharedMemory),"%d ", numbersArray[1]);
    }
    for (int i=2;i<n+1;i++){
      numbersArray[i]=fibonacciSequence(numbersArray[i-1],numbersArray[i-2]); 
        sprintf(sharedMemory + strlen(sharedMemory), "%d ", numbersArray[i]); 
    }
    shmdt(sharedMemory);
    
    exit(0); // Encerra o processo filho
  }else if (pid>0){
    wait(NULL);
    printf("Sou o processo pai, essa é a sequência de Fibonacci gerada pelo meu filho:\n");
    sharedMemory=(char *) shmat(segment_id, NULL,0);
    printf("%s\n",sharedMemory);
    shmctl(segment_id, IPC_RMID,NULL);
  }
  return 0;
}
