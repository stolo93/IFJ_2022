//htab_struct.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádzajú interné štruktúry
//htab.h pre úlohu b)
// 
#include "htab.h"

struct htab{
    size_t size;
    size_t arr_size;
    struct htab_item** ptrs;
};

struct htab_item{
    htab_pair_t pair;
    struct htab_item* next;
};