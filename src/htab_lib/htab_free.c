/****************************************************************
 * @name htab_free.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/ 
#include <stdio.h>
#include <stdlib.h>


#include "../headers/htab.h"
#include "../headers/htab_struct.h"

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