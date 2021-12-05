#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>

void my_sem_init(my_sem *sem, int pshared){
    *sem =malloc(sizeof(my_sem));
    sem->value= pshared;
    sem->mod= 0;
    sem->wait= 0;
}

void my_sem_destroy(my_sem *sem){
    free(sem);
}

void my_sem_wait(my_sem *sem){
    lock(&(sem->wait));
    while(sem->value<=0){}
    lock(&(sem->mod));
    (sem->value)--;
    unlock(&(sem->wait));
    unlock(&(sem->mod));
}

void my_sem_post{
    lock(&(sem->mod));
    (sem->value)++;
    unlock(&(sem->mod));        
}


void lock(int *verrou){
  while(*verrou==1){
    asm("movl $1, %%eax;"
    "xchgl %%eax, %1;"
    "movl %%eax,%0;"
        :"=r" (*verrou)
        :"r" (*verrou)
        :"%eax"
    );
  }
}

void unlock(int *verrou){
  int arg1=0;
  asm("movl $0, %%eax;"
    "xchgl %%eax, %1;"
    "movl %%eax,%0;"
        :"=r" (*verrou)
        :"r" (*verrou)
        :"%eax"
    );
}
