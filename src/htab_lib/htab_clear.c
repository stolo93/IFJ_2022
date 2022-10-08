//htab_clear.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_clear pre úlohu b)
// 
#include <stdio.h>
#include <stdlib.h>

#include "htab.h"
#include "htab_struct.h"

/** Function which clears all records from table
 *
 *  @param t table from which we want to erase all records
 *  
 ***/
void htab_clear(htab_t * t){

    if(!t){
        fprintf(stderr,"Table is invalid pointer\n");
        return;
    }

    for(size_t counter = 0; counter < t->arr_size; counter++){  //prechod hlavičkami zoznamov

        struct htab_item * tmp = t->ptrs[counter];

        while(tmp){   //prechod zoznamom a vymazávanie všetkých záznamov v ňom
            
            struct htab_item * to_erase = tmp;
            tmp = tmp->next;
            free((char*)to_erase->pair.key);
            free(to_erase);
        }
    }
    free(t->ptrs);   //uvedenie tabuľky to počiatočného stavu
    t->ptrs = NULL;
    t->size = 0;
    t->arr_size = 0;
    return;
}