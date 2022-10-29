/****************************************************************
 * @name htab_init.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../headers/htab.h"
#include "../headers/htab_struct.h"
#include "../headers/interner.h"

extern interner* interner_ptr ; 

//funkcia ktorá vytvorí a inicializuje novú tabuľku s počtom zoznamov num
/** Function which initialize hash table  
 *
 *  @param num number of lists which we want create
 *  @return pointer to the hash table or NULL if initialization fails
 ***/
error( htab_t_ptr ) htab_init(size_t num){

    if(num <= 0)
    {    
        return_error( ERROR_HTAB_INIT , htab_t_ptr );
    }
    htab_t* htab = (htab_t*)malloc(sizeof(htab_t));

    if(!htab)
    {    
        return_error(ERROR_MAL , htab_t_ptr );
    }
    htab->size = 0;
    htab->arr_size = num;

    htab->ptrs = (struct htab_item**) malloc(sizeof(struct htab_item*) * num);

    if(!htab->ptrs){
        
        free(htab);
        return_error(ERROR_MAL , htab_t_ptr );
    }
    for(size_t counter = 0; counter < num; counter++){

        htab->ptrs[counter] = NULL;
    }
    //strings for build in fnctions, \0 character is there beacause memcpy had problem if it wasn't there
    char functions[NUMBER_OF_BUILD_IN_FUNCTIONS][SIZE_OF_BUILD_IN_STRING] = { "reads\0", "readi\0", "readf\0", "write\0"};
    for( unsigned int counter = 0 ; counter < NUMBER_OF_BUILD_IN_FUNCTIONS ; counter++ )
    {
        const char* ptr = ( const char * ) malloc ( sizeof( char ) * SIZE_OF_BUILD_IN_STRING );
        
        if ( ptr == NULL )
        {
            free( htab->ptrs );
            free( htab );
            return_error( ERROR_MAL , htab_t_ptr );
        }
        memcpy( (char* ) ptr , functions[counter] , SIZE_OF_BUILD_IN_STRING );
        
        error( intern_id ) id = intern( interner_ptr , (char*)ptr ); //putting it inside interner
        if( is_error( id ))
        {
            free( htab->ptrs );
            free( htab );
            forward_error( id , htab_t_ptr );
        }
        get_value( htab_pair_t_ptr , newEntry , htab_lookup_add( htab , id._value , function ) , htab_t_ptr );
        if ( counter == 0)
        {   //giving each function its out data type 
            newEntry->diff.func.outType = stringTNull ;
        } 
        else if( counter == 1 )
        {
            newEntry->diff.func.outType = integerTNull;
        }
        else if ( counter == 2 )
        {
            newEntry->diff.func.outType = floatingTNull;
        }
        else 
        {
            newEntry->diff.func.outType = noType ;
        }
        
    }
    
    return_value( htab , htab_t_ptr );
}