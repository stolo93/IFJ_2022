/****************************************************************
 * @name htab_resize.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../headers/symtable.h"
#include "../headers/htab_struct.h"




/** Function which will resize number of lists 
 *
 *  @param newn count of new lists 
 *  @param t table which we want to resize
 ***/
error(none ) htab_resize(htab_t* t, size_t newn){

    if(!t || newn == 0){
        
        return_error( ERROR_HTAB_RESZ , none );
    }

    size_t old_size = t->arr_size;
    struct htab_item** old_ptrs = t->ptrs;

    t->arr_size = newn;
    t->ptrs = (struct htab_item**) calloc(sizeof(struct htab_item*),newn);
    
    if(!t->ptrs){                 //if allocation fails nothing happens and function is quiet about this fail
        t->arr_size = old_size;   
        t->ptrs = old_ptrs;
        
        return_none();
    }

    for(size_t counter = 0;counter < old_size; counter++){

        struct htab_item* tmp = old_ptrs[counter];           //one pointer from previous order of elements

        while(tmp){

            htab_key_t key = tmp->pair.key;                 // storing key for further use
            struct htab_item* next = tmp->next;             //pointer on next element of table

            unsigned long index = htab_hash_function(key) % t->arr_size;     //calculating new index

            if(!t->ptrs[index]){              //first entry on given index
                t->ptrs[index] = tmp;
                t->ptrs[index]->next = NULL;      //grounding so it won't point to previous next element
            } else {

                struct htab_item* tmp2 = t->ptrs[index];      //going through new list
                while(tmp2){

                    if(tmp2->next == NULL){       //if next is null I will store new entry
                        tmp->next = NULL;        //grounding
                        tmp2->next = tmp;

                        break;
                    }
                    tmp2 = tmp2->next;      //passing to next entry in new list
                }
            }
            tmp = next;   //passing to next entery in old list
        }

    }

    free(old_ptrs);
    return_none();
}