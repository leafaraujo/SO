#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_N 100

int n=10;

int main(int argc, char **argv){
  int segment_id=shmget(IPC_PRIVATE, 10*sizeof(int),S_IRUSR|S_IWUSR);
  int *sharedSum = (int *)shmat(segment_id, NULL, 0);
  *sharedSum = 0;
  printf("Sou o processo pai %d, vou criar um filho para fazer somas sucessivas.\n", getpid());
  fflush(stdout);
  for(int i = 1; i <= n; i++){
    pid_t pid = fork();
    if(pid == 0){
      printf("Sou o processo filho %d, e vou somar por aqui o valor %d!\n",getpid(), i);
      *sharedSum += i;
      exit(0);
    }
    else if(pid < 0){
      printf("Erro ao criar processo filho.\n");
      exit(1);
    }
    else{
      wait(NULL);
    }

  }
  wait(NULL);
  printf("O valor final da soma é: %d\n", *sharedSum);
  shmdt(sharedSum);
  shmctl(segment_id, IPC_RMID,NULL);
  
  return 0;
}
