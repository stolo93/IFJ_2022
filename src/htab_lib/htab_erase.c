//htab_erase.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_erase pre úlohu b)
// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"
#include "htab_struct.h"


#define AVG_LEN_MIN 40

//funkcia ktorá vymaže záznam v tabuľke s klúčom key
//ak priemerná dĺžka záznamov klesne pod AVG_LEN_MIN tak sa zmenší počet zoznamov na polovicu

/** Function for erasing one entry from table and changes size of table if neccesary
 *
 *  @param t table from which we want to erase entry
 *  @param key identifier of record
 *  @return succes of erasing
 ***/
bool htab_erase(htab_t * t, htab_key_t key){

    if(!t || !key){
        fprintf(stderr,"Table or key is invalid pointer\n");
        return false;
    }

    size_t index = htab_hash_function(key) % t->arr_size;

    struct htab_item* tmp = t->ptrs[index];            
    struct htab_item* trailing = t->ptrs[index];      //o jeden krok pozadu za tmp okrem prvého kroku, to rieši stav vymazania prvého a jediného záznamu

    while(tmp){

        if(!strcmp(tmp->pair.key,key)){

            free((char*)tmp->pair.key);
            
            if(tmp == t->ptrs[index]){

                struct htab_item* to_be_erased = trailing;  // prípad vymazania prvého prvku
                t->ptrs[index] = NULL;
                free(to_be_erased);
                t->size--;
                if(AVG_LEN_MIN > (t->size / t->arr_size)){    //kontrola priemernej dĺžky zoznamov
                    htab_resize(t,(size_t)(t->arr_size/2));
                }
                return true;
            }
            struct htab_item* to_be_erased = trailing->next;  // vybratie pointeru na uvoľnenie
            trailing->next = tmp->next;                      //nové prepojenie
            free(to_be_erased);
            t->size--;
            if(AVG_LEN_MIN > (t->size / t->arr_size)){   //kontrola priemernej dĺžky zoznamov
                htab_resize(t,(size_t)(t->arr_size/2));
            }
            return true;
        }
        trailing = tmp;
        tmp = tmp->next;
    }
    return false;
}