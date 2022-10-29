/****************************************************************
 * @name htab_erase.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/htab.h"
#include "../headers/htab_struct.h"


#define AVG_LEN_MIN 40

//funkcia ktorá vymaže záznam v tabuľke s klúčom key
//ak priemerná dĺžka záznamov klesne pod AVG_LEN_MIN tak sa zmenší počet zoznamov na polovicu

/** Function for erasing one entry from table and changes size of table if neccesary
 *
 *  @param t table from which we want to erase entry
 *  @param key identifier of record
 *  @return succes of erasing
 ***/
error(_Bool ) htab_erase(htab_t * t, char* key){

    if(!t || !key){
        
        return_error( ERROR_HTAB_INVPTR , bool);
    }

    size_t index = htab_hash_function(key) % t->arr_size;

    struct htab_item* tmp = t->ptrs[index];            
    struct htab_item* trailing = t->ptrs[index];      //one step behind tmp pointer , edgecase of deleting first entry

    while(tmp){

        if(!strcmp(tmp->pair.key,key)){

            free((char*)tmp->pair.key);
            if ( tmp->pair.symType == variable && tmp->pair.diff.var.dataType == stringT )
            {
                free( tmp->pair.diff.var.info.string ) ; // if there will be anything which takes string it shuold be freed here
            }
            else if ( tmp->pair.symType ==  function )
            {
               
                vec_structFuncParam_destroy( &tmp->pair.diff.func.inParams );
            }
            if(tmp == t->ptrs[index]){

                struct htab_item* to_be_erased = trailing;  // deleting first entry
                t->ptrs[index] = NULL;
                free(to_be_erased);
                t->size--;
                if(AVG_LEN_MIN > (t->size / t->arr_size)){    //checking average size of lists
                    htab_resize(t,(size_t)(t->arr_size/2));
                }
                return_value( true , bool );
            }
            struct htab_item* to_be_erased = trailing->next;  
            trailing->next = tmp->next;                      //new connection
            free(to_be_erased);
            t->size--;
            if(AVG_LEN_MIN > (t->size / t->arr_size)){   //checking average size of lists
                htab_resize(t,(size_t)(t->arr_size/2));
            }
            return_value( true , bool );
        }
        trailing = tmp;
        tmp = tmp->next;
    }
    return_value( false , bool );   
}