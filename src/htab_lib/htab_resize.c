//htab_resize.c                                                                  
//Riešenie IJC-DU2, príklad b), 19.4.2022
//Autor: Jozef Michal Bukas, FIT
//Preložené: gcc (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0
//Popis: Súbor v ktorom sa nachádza funkcia htab_resize pre úlohu b)
// 
#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "htab_struct.h"


//funkcia ktorá zmení počet zoznamov na hodnotu newn

/** Function which will resize number of lists 
 *
 *  @param newn count of new lists 
 *  @param t table which we want to resize
 ***/
void htab_resize(htab_t* t, size_t newn){

    if(!t || newn <= 0){
        fprintf(stderr,"table is null or newn is less then 1\n");
        return;
    }

    size_t old_size = t->arr_size;
    struct htab_item** old_ptrs = t->ptrs;

    t->arr_size = newn;
    t->ptrs = (struct htab_item**) calloc(sizeof(struct htab_item*),newn);
    
    if(!t->ptrs){                 //ak alokácia zlyhá tak sa všetko vráti do stavu pred volaním funkcie
        t->arr_size = old_size;   //funkcia o tomto zlyhaní zamlčí
        t->ptrs = old_ptrs;
        return;
    }

    for(size_t counter = 0;counter < old_size; counter++){

        struct htab_item* tmp = old_ptrs[counter];           //priradím jeden ukazateľ z prveho usporiadania

        while(tmp){

            htab_key_t key = tmp->pair.key;                 // uložím si klúč aktuálnej dvojice
            struct htab_item* next = tmp->next;             //uložím si ukazateľ na ďalší prvok

            size_t index = htab_hash_function(key) % t->arr_size;     //vyrátam nový index

            if(!t->ptrs[index]){              //ak na tom indexe ešte nič nieje uložené tak tam uložím záznam
                t->ptrs[index] = tmp;
                t->ptrs[index]->next = NULL;      //uzemnenie aby neukazoval na starý prvok
            } else {

                struct htab_item* tmp2 = t->ptrs[index];      //prechod novým usporiadaním
                while(tmp2){

                    if(tmp2->next == NULL){       //keď je next null tak tam uložím záznam
                        tmp->next = NULL;        //uzemnenie
                        tmp2->next = tmp;

                        break;
                    }
                    tmp2 = tmp2->next;      //predávanie ukazateľa
                }
            }
            tmp = next;   //prechod na dalšiu dvojicu
        }

    }

    free(old_ptrs);
    return;
}