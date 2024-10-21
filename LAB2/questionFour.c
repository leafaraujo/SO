#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <semaphore.h>

int numberOfThreads=5;
sem_t semaphore;

void *printHello(void *tid){
  printf("Olá, eu sou a thread %d\n", (int)(size_t) tid);
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  pthread_t threads[numberOfThreads];
  sem_init(&semaphore, 0, 1);
  for (int i=0;i<numberOfThreads;i++){
    int status=pthread_create(&threads[i],NULL,printHello,(void *)(size_t) i);

    if(status!=0){
      printf("Erro na criação da thread %d. Código de erro %d.",i, status);
      return 1;
    }

    sleep(2);
  }
  sem_destroy(&semaphore);
  return 0;
}
