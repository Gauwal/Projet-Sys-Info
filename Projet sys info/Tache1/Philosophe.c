#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int numb;
pthread_mutex_t *baguettes;
void mange(){
}

void *philosophe(void* arg){
  int *id=(int *) arg;
  int left = *id;
  int right = (left + 1) % numb;
  for(int i=0; i<100000;i++){
    if(left < right){
      pthread_mutex_lock(&baguettes[left]);
      pthread_mutex_lock(&baguettes[right]);
    }
    else{
      pthread_mutex_lock(&baguettes[right]);
      pthread_mutex_lock(&baguettes[left]);
    }
    mange();
    pthread_mutex_unlock(&baguettes[left]);
    pthread_mutex_unlock(&baguettes[right]);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  if(argc!=2){
    
    return EXIT_FAILURE;
    
  }
  numb=atoi(argv[1]);
  baguettes=malloc(numb*sizeof(pthread_mutex_t));
  pthread_t phil[numb];
  for(int i=0;i<numb;i++){
    pthread_mutex_init(&baguettes[i],NULL);
  }
  for(int j=0;j<numb;j++){
    pthread_create(&phil[j],NULL,philosophe,(void*) &(j));
  }
  for(int k=0;k<numb;k++){
    pthread_join(phil[k],NULL);
  }
  for(int l=0;l<numb;l++){
    pthread_mutex_destroy(&baguettes[l]);
  }
  free(baguettes);
  return EXIT_SUCCESS;
}
