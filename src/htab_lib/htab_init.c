//htab_init.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_init pre úlohu b)
// 
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "htab.h"
#include "htab_struct.h"

//funkcia ktorá vytvorí a inicializuje novú tabuľku s počtom zoznamov num
htab_t* htab_init(size_t num){

    if(num <= 0){
        fprintf(stderr,"Num must be greater or equal to 1\n");
        return NULL;
    }

    htab_t* htab = (htab_t*)malloc(sizeof(htab_t));

    if(!htab){
        fprintf(stderr,"malloc failed to allocate memory\n");
        return NULL;
    }
    htab->size = 0;
    htab->arr_size = num;

    htab->ptrs = (struct htab_item**) malloc(sizeof(struct htab_item*) * num);

    if(!htab->ptrs){
        fprintf(stderr,"malloc failed to allocate memory\n");
        free(htab);
        return NULL;
    }
    for(size_t counter = 0; counter < num; counter++){

        htab->ptrs[counter] = NULL;
    }

    return htab;
}