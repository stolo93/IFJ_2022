/****************************************************************
 * @name htab_clear.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
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
            if ( to_erase->pair.symType == variable && to_erase->pair.type == string )
            {
                free( to_erase->pair.info.string ) ; // if there will be anything which takes string it shuold be freed here
            }
            free(to_erase);
        }
    }
    free(t->ptrs);   //uvedenie tabuľky to počiatočného stavu
    t->ptrs = NULL;
    t->size = 0;
    t->arr_size = 0;
    return;
}