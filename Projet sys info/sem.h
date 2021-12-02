#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>

pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;

sem_t db; 
sem_t reader_block; 

int readcount=0; 
int writecount=0; 
int writing_nb = 640; 
int reading_nb = 2560; 


void write_database(){
}
void read_database(){
}
void waste_time(){
  while(rand() > RAND_MAX/10000);
}

void writer(void){
  while(true){
    waste_time();
    pthread_mutex_lock(&mutex_writecount);

    if(writing_nb <= 0){
      pthread_mutex_unlock(&mutex_writecount);
      break;
    }
    else
      writing_nb--;
    writecount++;
    if(writecount==1)
      sem_wait(&reader_block);

    pthread_mutex_unlock(&mutex_writecount);

    sem_wait(&db);

    write_database();
    sem_post(&db);

    pthread_mutex_lock(&mutex_writecount);

    writecount--;
   
    if(writecount==0)
      sem_post(&reader_block);

    pthread_mutex_unlock(&mutex_writecount);
  }
}

void reader(void){
  while(true){
    sem_wait(&reader_block);
    pthread_mutex_lock(&mutex_readcount);

    if(reading_nb <= 0){
      pthread_mutex_unlock(&mutex_readcount);
      sem_post(&reader_block);
      break;
    }
    else
      reading_nb--;
    readcount++;
    if(readcount==1)
      sem_wait(&db);

    pthread_mutex_unlock(&mutex_readcount);
    sem_post(&reader_block);
    read_database();
    pthread_mutex_lock(&mutex_readcount);

    readcount--;
    if(readcount==0)
      sem_post(&db);

    pthread_mutex_unlock(&mutex_readcount);
    waste_time();
  }
}


int main(int argc, char *argv[]){
  if(argc != 3){
    return EXIT_FAILURE;
  }
  int nb_writers = atoi(argv[1]);
  int nb_readers = atoi(argv[2]);
  int err;
  int i;
  
  pthread_t writers[nb_writers];
  pthread_t readers[nb_readers];

   for(i=0; i<nb_writers; i++){
     pthread_create(&writers[i], NULL,(void *) writer, NULL);
   }
   for(i=0; i<nb_readers; i++){
     pthread_create(&readers[i], NULL, (void *)reader, NULL);
   }
   
   for(i=0; i<nb_writers; i++){
     pthread_join(writers[i], NULL);
   }
   for(i=0; i<nb_readers; i++){
     pthread_join(readers[i], NULL);
   }
   
   pthread_mutex_destroy(&mutex_readcount);
   pthread_mutex_destroy(&mutex_writecount);
   
   sem_destroy(&reader_block);
   sem_destroy(&db);
   return EXIT_SUCCESS;
}
