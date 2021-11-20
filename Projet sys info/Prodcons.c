#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

pthread_mutex_t prod_done;
pthread_mutex_t cons_done;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
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
    pthread_mutex_lock(&prod_done);
    if(nbprod<=0){
      pthread_mutex_unlock(&prod_done);
      break;
    }
    nbprod--;
    pthread_mutex_unlock(&prod_done);
    
    item=produce(item);
    sem_wait(&empty); // attente d'une place libre
    pthread_mutex_lock(&mutex);
    
    buffer[point]=item;
    point=(point+1)%8;
    
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a une place remplie en plus
  }
  return NULL;
}

void *consumer()
{
 int item;
 while(true)
 {
    pthread_mutex_lock(&cons_done);
    if(nbcons<=0){
      pthread_mutex_unlock(&cons_done);
      break;
    }
    nbcons--;
    pthread_mutex_unlock(&cons_done);   
    
    sem_wait(&full); // attente d'une place remplie
    pthread_mutex_lock(&mutex);
    
    item=buffer[carre];
    carre=(carre+1)%8;
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty); // il y a une place libre en plus
    consume(item);
 }
 return NULL;
}


int main(int argc, char* argv[]){
  int nb_producer=atoi(argv[2]);
  int nb_consumer=atoi(argv[1]);
  
  sem_init(&empty, 0 , N);  // buffers vides
  sem_init(&full, 0 , 0); 

  pthread_mutex_init(&prod_done, NULL);
  pthread_mutex_init(&cons_done, NULL);
  pthread_mutex_init(&mutex, NULL); 
  
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
  pthread_mutex_destroy(&prod_done);
  pthread_mutex_destroy(&cons_done);
  pthread_mutex_destroy(&mutex);
  return EXIT_SUCCESS;  
}
