/****************************************************************
 * @name htab_init.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "htab.h"
#include "htab_struct.h"

//funkcia ktorá vytvorí a inicializuje novú tabuľku s počtom zoznamov num
/** Function which initialize hash table  
 *
 *  @param num number of lists which we want create
 *  @return pointer to the hash table or NULL if initialization fails
 ***/
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