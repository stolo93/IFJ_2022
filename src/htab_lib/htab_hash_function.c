/****************************************************************
 * @name htab_hash_function.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../headers/htab.h"
#include "../headers/htab_struct.h"

/** Hash function which is used by table
 *
 *  @param str key from entry
 *  @return Hash. Hash modulo arr_size will return index into hash table
 ***/
unsigned long  htab_hash_function(const char *str) {
          
    /*uint32_t h=0;     // must have 32 bits
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;*/

    unsigned long hash =  5381 ;
    int c;

    while( (c = *str++ ))
    {
        hash = ((hash << 5) + hash ) + c;
    }

    return hash;
}