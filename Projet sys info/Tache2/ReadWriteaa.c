#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sem.h"

int mutex_readcount;
int mutex_writecount;

my_sem db; 

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
    
    my_lock(&mutex_writecount);

    if(writing_nb <= 0){
      my_unlock(&mutex_writecount);
      break;
    }
    else
      writing_nb--;
    writecount++;
    my_unlock(&mutex_writecount);

    my_sem_wait(&db);
    write_database();
    my_sem_post(&db);

    my_lock(&mutex_writecount);

    writecount--;

    my_unlock(&mutex_writecount);
  }
}

void reader(void){
  while(true){
    my_lock(&mutex_readcount);

    if(reading_nb <= 0){
      my_unlock(&mutex_readcount);
      break;
    }
    else
      reading_nb--;
    readcount++;
    if(readcount==1){
      my_sem_wait(&db);}

    my_unlock(&mutex_readcount);
    read_database();
    my_lock(&mutex_readcount);

    readcount--;
    if(readcount==0){
      my_sem_post(&db);}

    my_unlock(&mutex_readcount);
    
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

  
  my_sem_init(&db, 0);
  mutex_readcount=0;
  mutex_writecount=0;
  
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

   //my_sem_destroy(&db);
   

   return EXIT_SUCCESS;
}
