//htab_free.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_free pre úlohu b)
// 
#include <stdio.h>
#include <stdlib.h>


#include "htab.h"
#include "htab_struct.h"

//funkcia ktorá uvoľní celú tabuľku
/** Function which frees whole table 
 *
 *  @param t table which we want to free
 *  
 ***/
void htab_free(htab_t * t){

    if(!t){
        fprintf(stderr,"Table is invalid pointer\n");
        return;
    }
    htab_clear(t);  //uvoľnenie všetkých záznamov
    free(t);        //uvoľnenie tabuľky
    return;
}