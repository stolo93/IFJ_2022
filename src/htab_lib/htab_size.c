/****************************************************************
 * @name htab_size.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>

#include "../headers/htab.h"
#include "../headers/htab_struct.h"

/** Function which returns size of table 
 *
 *  @param t table whose size we want
 *  @return size of table or -1 if t is invalid pointer
 ***/
size_t htab_size(const htab_t * t){

    if(!t){
        fprintf(stderr,"Table isn't valid pointer\n");
        return -1;
    }

    return t->size;
}

/** Function which returns bucket count of table 
 *
 *  @param t table whose bucket count we want
 *  @return bucket count of table or -1 if t is invalid pointer
 ***/
size_t htab_bucket_count(const htab_t * t){

    if(!t){
        fprintf(stderr,"Table isn't valid pointer\n");
        return -1;
    }
    return t->arr_size;
}