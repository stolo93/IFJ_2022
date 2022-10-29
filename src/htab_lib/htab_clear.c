/****************************************************************
 * @name htab_clear.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "../headers/htab.h"
#include "../headers/htab_struct.h"

/** Function which clears all records from table
 *
 *  @param t table from which we want to erase all records
 *  
 ***/
error(none) htab_clear(htab_t * t){

    if(!t){
        
        return_error(ERROR_HTAB_INVPTR, none);
    }

    for(size_t counter = 0; counter < t->arr_size; counter++){  

        struct htab_item * tmp = t->ptrs[counter];

        while(tmp){   
            
            struct htab_item * to_erase = tmp;
            tmp = tmp->next;
            /*free((char*)to_erase->pair.key);
            if ( to_erase->pair.symType == variable && to_erase->pair.diff.var.dataType == stringT )
            {
                free( to_erase->pair.diff.var.info.string ) ; // if there will be anything which takes string it should be freed here
            }
            else*/ 
            if ( to_erase->pair.symType == function )
            {
                vec_structFuncParam_destroy( &to_erase->pair.diff.func.inParams );
            }
            free(to_erase);
        }
    }
    free(t->ptrs);   
    t->ptrs = NULL;
    t->size = 0;
    t->arr_size = 0;
    return_none();
}