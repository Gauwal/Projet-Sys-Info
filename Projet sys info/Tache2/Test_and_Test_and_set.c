#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int *verrou ;
int numb;

void lock(int *verrou){
  
  asm("loop:"
  
      "movl $1, %%eax;"
      "xchgl %%eax, %1;"
      
      "testl %%eax, %%eax;"
      "jnz loop2;"
      "jmp end;"
      
      
      "loop2:"
      
      "testl $1, %1;"
      "jnz loop2;"
      "jmp loop;"
      
      
      "end:"
      
      : "=m" (*verrou)
      : "m" (*verrou)
      : "%eax"
      );
      
  
  
}

void unlock(int *verrou){
  
  asm("movl $0, %%eax;"
    "xchgl %%eax, %1;"
    
        :"=m" (*verrou)
        :"m" (*verrou)
        :"%eax"
    );
    
    
    
}

int sect_crit(){
  while(rand() > RAND_MAX/10000); //random time de production
  return 1;
}

void * work(){
  for (int i=0;i<(6400/numb);i++){
    lock(verrou);
    sect_crit();
    unlock(verrou);
  }; 
  return NULL;
}

int main(int argc, char *argv[]){
  if(argc!=2){
    
    return EXIT_FAILURE;
    
  }
  numb=atoi(argv[1]);
  verrou=malloc(sizeof(int));
  *verrou=0;
  pthread_t thread[numb];
  
  for(int j=0;j<numb;j++){
    pthread_create(&thread[j],NULL,work,(void*) &(j));
  }
  
  for(int k=0;k<numb;k++){
    pthread_join(thread[k],NULL);
  }
  

  free(verrou);
  return EXIT_SUCCESS;
}
