#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void lock(int *verrou){
  
  while(*verrou==1){
    asm("movl $1, %%eax;"
    "xchgl %%eax, %1;"
    "movl %%eax,%0;"
        :"=r" (*verrou)
        :"r" (*verrou)
        :"%eax"
    );
  }
}

void unlock(int *verrou){
  int arg1=0;
  asm("movl $0, %%eax;"
    "xchgl %%eax, %1;"
    "movl %%eax,%0;"
        :"=r" (*verrou)
        :"r" (*verrou)
        :"%eax"
    );
}

