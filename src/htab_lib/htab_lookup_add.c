/****************************************************************
 * @name htab_lookup_add.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/htab.h"
#include "../headers/htab_struct.h"

#define AVG_LEN_MAX 150

//funkcia ktorá vytvorí nový záznam do zoznamu ktorý bude identifikovaný klúčom key

/** Function which creates new intem for hash table
 *
 *  @param key identifier of entry
 *  @return pointer to the new item or NULL pointer if internal error occurs
 ***/
struct htab_item* create_new_item(htab_key_t key){

    struct htab_item* new_item = (struct htab_item* ) malloc(sizeof(struct htab_item));
    
    if(!new_item){
        fprintf(stderr,"Malloc failed to allocate memory\n");
        return NULL;
    }

    char* new_key = (char*) malloc(sizeof(char) * strlen(key)+1);

    if(!new_key){
        fprintf(stderr,"Malloc failed to allocate memory\n");
        free(new_item);
        return NULL;
    }

    memcpy((char*)new_key,(char*)key,strlen(key)+1);  //prekopírovanie klúča do záznamu
    new_item->pair.key = new_key;                     //ukazatele treba počas kopýrovania pretypovať              
    new_item->pair.val = 1;
    new_item->pair.type = notDefined;
    new_item->pair.symType = function;
    new_item->pair.info.integer = 0;         //OVERHERE i changed initialization
    new_item->next = NULL;

    return new_item;
}

//funkcia ktorá vyhľadá záznam s hodnotou klúč a inkrementuje jeho hodnotu
//ak nenájde hľadaný záznam tak ho vytvorí a pripojí ho na koniec zoznamu
//v prípade ak priemerná dĺžka zoznamov prekročí AVG_LEN_MAX tak sa počet zoznamov zdvojnásobi

/** Function which finds entry and increments counter or if it fails it creates new entry 
 *
 *  @param t table where we want increment or find new entry
 *  @param key identifier of entry
 *  @return pointer to the entry or NULL if internal error occurs
 ***/
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){

    if(!t || !key){
        fprintf(stderr,"Key or table is NULL pointer\n");
        return NULL;
    }
    if(!t->ptrs){
        fprintf(stderr,"Firstly create bucket then add new items\n");
        return NULL;
    }
    size_t index = htab_hash_function(key) % t->arr_size;  //zízkanie indexu zoznamu v ktorom má hľadať
    
    struct htab_item * tmp = t->ptrs[index];
    struct htab_item * trailing = NULL;

    while(tmp){

        if(!strcmp(tmp->pair.key,key)){  //nájdenie záznamu

            tmp->pair.val++;   
            return &tmp->pair;
        }
        trailing = tmp;    
        tmp = tmp->next;
    }

    struct htab_item * new_item = create_new_item(key); //vytvorenie nového záznamu
    if(!new_item){
        return NULL;
    }

    if(!trailing){        //pripájanie nového záznamu

        t->ptrs[index] = new_item;
        
    } else {

        trailing->next = new_item;
    }
    t->size++;
    if(AVG_LEN_MAX < (t->arr_size / t->size)){   //kontrola priemernej dĺžky záznamov
        htab_resize(t,2*t->arr_size);
    }
    return &new_item->pair;
}