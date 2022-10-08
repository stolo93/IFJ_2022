//htab_find.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_find pre úlohu b)
// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"
#include "htab_struct.h"

//funkcia ktorá nájde záznam s klúčom key a vráti ukazateľ naň
/** Function for finding record 
 *
 *  @param t table where we want to find entry
 *  @param key identifier of entry
 *  @return pointer to the entry or NULL pointer if entry don't exists
 ***/
htab_pair_t * htab_find(htab_t * t, htab_key_t key){

    if(!t || !key){
        fprintf(stderr,"Key or table is NULL pointer\n");
        return NULL;
    }
    if(!t->ptrs){
        fprintf(stderr,"Bucket doesn't exist\n");
        return NULL;
    }

    size_t index = htab_hash_function(key) % t->arr_size; //nájdenie indexu zoznamu v ktorom by sa mal nachádzať daný záznam
    
    struct htab_item * tmp = t->ptrs[index];

    while(tmp){

        if(!strcmp(tmp->pair.key,key)) return &tmp->pair;
                
        tmp = tmp->next;
    }

    return NULL;
}