//htab_hash_function.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_hash_function pre úlohu b)
// 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "htab.h"
#include "htab_struct.h"

/** Hash function which is used by table
 *
 *  @param str key from entry
 *  @return Hash. Hash modulo arr_size will return index into hash table
 ***/
size_t htab_hash_function(const char *str) {
          
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}