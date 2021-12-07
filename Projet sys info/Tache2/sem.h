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

void my_sem_destroy(my_sem *sem);

void my_lock(int *verrou);

void my_unlock(int *verrou);

void my_lock_tatas(int *verrou);

void my_sem_wait_tatas(my_sem *sem);

void my_sem_post_tatas(my_sem *sem);
