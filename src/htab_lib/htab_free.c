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
error(none ) htab_free(htab_t * t){

    if(!t){
        return_error( ERROR_HTAB_INVPTR , none );
    }
    htab_clear(t);  
    free(t);        
    return_none();
}