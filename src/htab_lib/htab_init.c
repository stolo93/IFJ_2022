/****************************************************************
 * @name htab_init.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../headers/htab.h"
#include "../headers/htab_struct.h"

//funkcia ktorá vytvorí a inicializuje novú tabuľku s počtom zoznamov num
/** Function which initialize hash table  
 *
 *  @param num number of lists which we want create
 *  @return pointer to the hash table or NULL if initialization fails
 ***/
error( htab_t_ptr ) htab_init(size_t num){

    if(num <= 0){
        
        return_error( ERROR_HTAB_INIT , htab_t_ptr );
    }

    htab_t* htab = (htab_t*)malloc(sizeof(htab_t));

    if(!htab){
        
        return_error(ERROR_MAL , htab_t_ptr );
    }
    htab->size = 0;
    htab->arr_size = num;

    htab->ptrs = (struct htab_item**) malloc(sizeof(struct htab_item*) * num);

    if(!htab->ptrs){
        
        free(htab);
        return_error(ERROR_MAL , htab_t_ptr );
    }
    for(size_t counter = 0; counter < num; counter++){

        htab->ptrs[counter] = NULL;
    }

    return_value( htab , htab_t_ptr );
}