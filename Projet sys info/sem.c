#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include "sem.h"

void my_sem_init(my_sem *sem, int pshared){
    sem = malloc(sizeof(sem));
    sem->value= pshared;
    sem->mod= 0;
    sem->wait= 0;
}

void my_sem_destroy(my_sem *sem){
    free(sem);
}

void my_sem_wait(my_sem *sem){
    my_lock(&(sem->wait));
    while(sem->value<=0){}
    my_lock(&(sem->mod));
    (sem->value)--;
    my_unlock(&(sem->wait));
    my_unlock(&(sem->mod));
}

void my_sem_post(my_sem *sem){
    my_lock(&(sem->mod));
    (sem->value)++;
    my_unlock(&(sem->mod));        
}


void my_lock(int *verrou){
  
  asm("loop:\t"
      "movl $1, %%eax;"
      "xchgl %%eax, %1;"
      
      "testl %%eax, %%eax;"
      "jnz loop\t;"
      : "=m" (*verrou)
      : "m" (*verrou)
      : "%eax"
      );
      
  
  
}

void my_unlock(int *verrou){
  
  asm("movl $0, %%eax;"
    "xchgl %%eax, %1;"
    
        :"=m" (*verrou)
        :"m" (*verrou)
        :"%eax"
    );
    
    
    
}
