/****************************************************************
 * @name htab_find.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/htab.h"
#include "../headers/htab_struct.h"

/** Function for finding record 
 *
 *  @param t table where we want to find entry
 *  @param key identifier of entry
 *  @return pointer to the entry or NULL pointer if entry don't exists
 ***/
error(htab_pair_t_ptr) htab_find(htab_t * t, htab_key_t key){

    if(!t || !key){
        
        return_error( ERROR_HTAB_INVPTR , htab_pair_t_ptr);
    }
    if(!t->ptrs){
       
        return_error( ERROR_HTAB_INVBPTR , htab_pair_t_ptr);
    }

    size_t index = htab_hash_function(key) % t->arr_size; 
    
    struct htab_item * tmp = t->ptrs[index];

    while(tmp){

        if( !strcmp(tmp->pair.key,key) ) return_value( &tmp->pair , htab_pair_t_ptr );
                
        tmp = tmp->next;
    }

    return_value( NULL , htab_pair_t_ptr );//return_error(ERROR_HTAB_NKEY , htab_pair_t_ptr );
}