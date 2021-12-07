#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "sem.h"

int prod_done;
int cons_done;
int mutex;
my_sem empty;
my_sem full;
#define N 8


int nbprod=1024;
int nbcons=1024;
int buffer[8];
int point=0;
int carre=0;

int produce(){
  while(rand() > RAND_MAX/10000); //random time de production
  return rand();
}

int consume(int item){
  while(rand() > RAND_MAX/10000); //random time de production
  return item;
}

void *producteur(){
  int item;
  while(true)
  {
    my_lock(&prod_done);
    if(nbprod<=0){
      my_unlock(&prod_done);
      break;
    }
    nbprod--;
    my_unlock(&prod_done);
    
    item=produce(item);
    my_sem_wait(&empty); // attente d'une place libre
    my_lock(&mutex);
    
    buffer[point]=item;
    point=(point+1)%8;
    
    my_unlock(&mutex);
    my_sem_post(&full); // il y a une place remplie en plus
  }
  return NULL;
}

void *consumer()
{
 int item;
 while(true)
 {
    my_lock(&cons_done);
    if(nbcons<=0){
      my_unlock(&cons_done);
      break;
    }
    nbcons--;
    my_unlock(&cons_done);   
    
    my_sem_wait(&full); // attente d'une place remplie
    my_lock(&mutex);
    
    item=buffer[carre];
    carre=(carre+1)%8;
    
    my_unlock(&mutex);
    my_sem_post(&empty); // il y a une place libre en plus
    consume(item);
 }
 return NULL;
}


int main(int argc, char* argv[]){
  int nb_producer=atoi(argv[2]);
  int nb_consumer=atoi(argv[1]);
  
  my_sem_init(&empty, N);  // buffers vides
  my_sem_init(&full, 0); 

  prod_done=0;
  cons_done=0;
  mutex=0;

  pthread_t c[nb_consumer];
  pthread_t p[nb_producer];
  for(int j=0;j<nb_consumer;j++){
    pthread_create(&c[j],NULL,consumer,NULL);
  }
  
  for(int j=0;j<nb_producer;j++){
    pthread_create(&p[j],NULL,producteur,NULL);
  }
  
  for(int k=0;k<nb_producer;k++){
    pthread_join(p[k],NULL);
  }
  for(int k=0;k<nb_consumer;k++){
    pthread_join(c[k],NULL);
  }

  return EXIT_SUCCESS;  
}
