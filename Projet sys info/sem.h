#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>


typedef struct my_sem{
    int value;
    int mod;
    int wait;
}my_sem;


void my_sem_init(my_sem *sem, int pshared);

void my_sem_wait(my_sem *sem);

void my_sem_post(my_sem *sem);
