#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;

sem_t db; 

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
    pthread_mutex_unlock(&mutex_writecount);

    sem_wait(&db);
    write_database();
    sem_post(&db);

    pthread_mutex_lock(&mutex_writecount);

    writecount--;

    pthread_mutex_unlock(&mutex_writecount);
  }
}

void reader(void){
  while(true){
    pthread_mutex_lock(&mutex_readcount);

    if(reading_nb <= 0){
      pthread_mutex_unlock(&mutex_readcount);
      break;
    }
    else
      reading_nb--;
    readcount++;
    if(readcount==1){
      sem_wait(&db);}

    pthread_mutex_unlock(&mutex_readcount);
    read_database();
    pthread_mutex_lock(&mutex_readcount);

    readcount--;
    if(readcount==0){
      sem_post(&db);}

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
  int i;

  
  sem_init(&db, 0, 1);
  pthread_mutex_init(&mutex_readcount, NULL);
  pthread_mutex_init(&mutex_writecount, NULL);
  
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

   sem_destroy(&db);
   

   return EXIT_SUCCESS;
}
