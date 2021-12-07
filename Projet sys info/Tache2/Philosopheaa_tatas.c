#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sem.h"




int numb;
int *baguettes;
void mange(){
}

void *philosophe(void* arg){
  int *id=(int *) arg;
  int left = *id;
  int right = (left + 1) % numb;
  for(int i=0; i<100000;i++){
    if(left < right){
      my_lock_tatas(&baguettes[left]);
      my_lock_tatas(&baguettes[right]);
    }
    else{
      my_lock_tatas(&baguettes[right]);
      my_lock_tatas(&baguettes[left]);
    }
    mange();
    my_unlock(&baguettes[left]);
    my_unlock(&baguettes[right]);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  if(argc!=2){
    
    return EXIT_FAILURE;
    
  }
  numb=atoi(argv[1]);
  baguettes=malloc(numb*sizeof(int));
  pthread_t phil[numb];
  for(int i=0;i<numb;i++){
    baguettes[i]=0;
  }
  for(int j=0;j<numb;j++){
    pthread_create(&phil[j],NULL,philosophe,(void*) &(j));
  }
  for(int k=0;k<numb;k++){
    pthread_join(phil[k],NULL);
  }
  free(baguettes);
  return EXIT_SUCCESS;
}
