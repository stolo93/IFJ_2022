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

//funkcia ktorá nad všetkými záznamamy vykoná funkciu f
/** Function which executes function on all entries from table
 *
 *  @param t table where we want execute function 
 *  @param f pointer to function which will be executed on all entries.
 *           This function can't alter any information in entry
 ***/
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){

    if(!t){
        fprintf(stderr,"Table is null pointer\n");
        return;
    }
    for(size_t counter = 0; counter < t->arr_size; counter++){

        struct htab_item* tmp = t->ptrs[counter];

        while(tmp){
            htab_key_t control_key = tmp->pair.key;
            htab_key_t text = (htab_key_t) malloc(sizeof(char) * strlen(tmp->pair.key)+1);  //vytváranie zálohy ak by funkcia zmenila klúč

            memcpy((char*)text,tmp->pair.key,strlen(tmp->pair.key)+1);

            (*f)(&tmp->pair);

            if(control_key != tmp->pair.key){         //kontrola zmeny adresy klúča

                fprintf(stderr,"Function tried relocate or erase key\n");
                
                if(tmp->pair.key) free((char*)tmp->pair.key);     //uvoľnenie novej nežiadúcej adresy
                
                tmp->pair.key = control_key;     //uloženie zálohy do zoznamu
                free((char*)text);
                return;

            }else if(strcmp(text,tmp->pair.key)){

                free((char*)tmp->pair.key);
                tmp->pair.key = text;       //zámena za pôvodný klúč
                
                fprintf(stderr,"Function tried to tamper with key\nAddres of key has changed but value of key is still same\n");
                return;
            }
            free((char*)text);
            tmp = tmp->next;
        }
    }
    return;
}