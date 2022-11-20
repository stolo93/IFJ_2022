/****************************************************************
 * @name htab_init.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../headers/htab.h"
#include "../headers/htab_struct.h"
#include "../headers/interner.h"

extern interner* interner_ptr ; 

/** Function which interns names of buildin functions  
 *
 *  @param ptrs array where interned ptrs should be stored
 *  @return error if interning fails
 ***/
error( none) intern_functions( char* ptrs[])
{
    char strings[NUMBER_OF_BUILD_IN_STRINGS][ SIZE_OF_BUILD_IN_STRING ] = { "reads", "readi", "readf", "write",
                                                                            "strlen", "substring", "ord", "chr",
                                                                            "$i","$j","$s"};
    
    for( unsigned int counter = 0 ; counter < NUMBER_OF_BUILD_IN_STRINGS ; counter++ )
    {
        get_value( intern_id , str , intern_copy( interner_ptr , strings[counter]) , none );
        ptrs[counter] = (char*)str;
    }
    return_none() ;
}

/** Function which adds buildin functions hash table  
 *
 *  @param htab pointer to table where we want to add buildin functions
 *  @return pointer to the hash table or error if adding fails
 ***/
error( htab_t_ptr ) add_Functions( htab_t_ptr htab)
{
    static const char* ptrs[11];
    static bool firstcall = true;
    error( none ) success;

    if ( firstcall == true)
    {
        firstcall = false;
        success = intern_functions( (char**)&(ptrs));
        if( is_error( success ))
        {
            forward_error( success , htab_t_ptr);
        }
    }
    structFuncParam param = { .inIdent = (char*) ptrs[POS_S] , .inType = stringT };
    structFuncParam param1 = { .inIdent = (char*) ptrs[POS_J] , .inType = integerT };
    structFuncParam param2 = { .inIdent = (char*) ptrs[POS_I] , .inType = integerT };

    for( unsigned int counter = 0 ; counter < NUMBER_OF_BUILD_IN_FUNCTIONS ; counter++ )
    {
        get_value( htab_pair_t_ptr , newEntry , htab_lookup_add( htab , ptrs[counter] , function ) , htab_t_ptr );
        
        if( strcmp( newEntry->key , "reads") == 0)
        {
            newEntry->diff.func.outType = stringTNull;
        }
        else if( strcmp( newEntry->key , "readi") == 0)
        {
            newEntry->diff.func.outType = integerTNull;
        }
        else if( strcmp( newEntry->key , "readf") == 0)
        {
            newEntry->diff.func.outType = floatingTNull;
        }
        else if( strcmp( newEntry->key , "write") == 0)
        {
            newEntry->diff.func.outType = noType;
        }
        else if( strcmp( newEntry->key , "strlen") == 0)
        {
            newEntry->diff.func.outType = integerT;
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param );
            if( is_error( success ))
            {
                forward_error(success , htab_t_ptr);
            }
        }
        else if( strcmp( newEntry->key , "substring") == 0)
        {
            newEntry->diff.func.outType = stringTNull;
            
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param );
            if( is_error( success ))
            {
                forward_error(success , htab_t_ptr);
            }
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param1 );
            if( is_error( success ))
            {
                forward_error(success , htab_t_ptr);
            }
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param2 );
            if( is_error( success ))
            {
                forward_error(success , htab_t_ptr);
            }
        }
        else if( strcmp( newEntry->key , "ord") == 0)
        {
            newEntry->diff.func.outType = integerT;
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param );
            if( is_error( success ))
            {
                forward_error(success , htab_t_ptr);
            }
        }
        else if( strcmp( newEntry->key , "chr") == 0)
        {
            newEntry->diff.func.outType = stringT;
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param1 );
            if( is_error( success ))
            {
                forward_error(success , htab_t_ptr);
            }
        }
    }
    return_value( htab , htab_t_ptr );
}

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
   
    get_value( htab_t_ptr , newTable , add_Functions( htab ), htab_t_ptr);
    htab = newTable;
    return_value( htab , htab_t_ptr );
}