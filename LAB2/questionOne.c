#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int value = 80;
int main(int argc, char **argv){
  int segment_id=shmget(IPC_PRIVATE, 10*sizeof(int),S_IRUSR|S_IWUSR);
  int *sharedMemory;
  printf("Sou o processo %d e o valor da vaiável é %d\n", getpid(), value);
  pid_t pidSon=fork(); 
  if(pidSon<0){ 
    printf("Sou o processo %d e nao consegui criar um novo processo.\n",getpid());
    return 1;
  }else if(pidSon==0){
    
    sharedMemory=(int *) shmat(segment_id, NULL,0);
    printf("Olá, processo pai, sou o seu neto e o meu ID é %d e adicionei %d ao valor que o pai me deu. ",getpid(), value);
    value += value;
    printf("O valor agora é: %d\n", value);
    *sharedMemory = value;
    pid_t pidUncle=fork();
    if(pidUncle<0){
      printf("Sou o processo %d e nao consegui criar um novo processo.\n",getpid());
      return 1;
    }
    else if(pidUncle == 0){
      printf("Olá, processo pai, sou o seu neto e o meu ID é %d e estou adicionando %d ao valor que o filho me deu. ",getpid(), value);
      value += value;
      printf("O valor agora é: %d\n", value);
      *sharedMemory = value;
      wait(NULL);
      exit(0);
    }
    
    shmdt(sharedMemory);
    wait(NULL);
    exit(0);
  }else{ 
    
    wait(NULL);
    
    sharedMemory=(int *) shmat(segment_id, NULL,0);
    
    printf("Olá novamente, sou o processo pai e o valor é: %d\n", *sharedMemory);

    shmctl(segment_id, IPC_RMID,NULL);
  }
  return 0;
}
