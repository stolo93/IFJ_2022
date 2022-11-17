/****************************************************************
 * @name htab_for_each.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/ 
#include <stdio.h>
#include <stdlib.h>


#include "../headers/htab.h"
#include "../headers/htab_struct.h"

/** Function which executes function on all entries from table
 *
 *  @param t table where we want execute function 
 *  @param f pointer to function which will be executed on all entries.
 *           This function can't alter any information in entry
 ***/
error(_Bool ) htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){

    if(!t){
        
        return_error( ERROR_HTAB_INVPTR , bool );
    }
    for(size_t counter = 0; counter < t->arr_size; counter++){

        struct htab_item* tmp = t->ptrs[counter];

        while(tmp){
            htab_key_t control_key = tmp->pair.key;
            htab_key_t text = (htab_key_t) malloc(sizeof(char) * strlen(tmp->pair.key)+1);  //creating copy of key if function tampers with it
            if ( text == NULL )
            {
                return_error( ERROR_MAL , bool );
            }

            memcpy((char*)text,tmp->pair.key,strlen(tmp->pair.key)+1);

            (*f)(&tmp->pair);

            if(control_key != tmp->pair.key) {         //check if key address is different

                if(tmp->pair.key) { free((char*)tmp->pair.key); }    //free unwanted address
                
                tmp->pair.key = (char*)control_key;     //copy into table
                free((char*)text);
                return_value(false , bool );

            } else if(strcmp(text,tmp->pair.key) != 0){

                free((char*)tmp->pair.key);
                tmp->pair.key = (char*)text;       //restoring right key
                
                
                return_value(false , bool );
            }
            free((char*)text);
            tmp = tmp->next;
        }
    }
    return_value(true , bool );
}