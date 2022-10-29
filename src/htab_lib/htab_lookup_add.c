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

/** Function which creates new intem for hash table identified by key
 *
 *  @param key identifier of entry
 *  @return pointer to the new item or NULL internal error occurs
 ***/
error( htab_item_ptr ) _create_new_item(htab_key_t key , sType type ){

    struct htab_item* new_item = (struct htab_item* ) malloc(sizeof(struct htab_item));
    
    if(!new_item){
       
        return_error( ERROR_MAL , htab_item_ptr);
    }

    char* new_key = (char*) malloc(sizeof(char) * strlen(key)+1); //this may be gone due to using same pointer for same string !!!

    if(!new_key){
        
        free(new_item);
        return_error( ERROR_MAL , htab_item_ptr );
    }

    memcpy((char*)new_key,(char*)key,strlen(key)+1);  //copying key into new entry
    new_item->pair.key = new_key;                                   
    new_item->pair.symType = type;
    new_item->pair.redefined = false;     
    if( type == variable )
    {
        new_item->pair.diff.var.dataType = notDefined;
        new_item->pair.diff.var.info.integer = 0;
    }
    else
    {
        new_item->pair.diff.func.outType = notDefined;
        
        get_value( vec_structFuncParam , newVec , new_vec_structFuncParam_with_capacity( 3 ) , htab_item_ptr);
        new_item->pair.diff.func.inParams = newVec;
    }
    new_item->next = NULL;

    return_value( new_item , htab_item_ptr );
}


//v prípade ak priemerná dĺžka zoznamov prekročí AVG_LEN_MAX tak sa počet zoznamov zdvojnásobi

/** Function which finds entry and increments counter or if it fails it creates new entry.   
 *
 *  @param t table where we want increment or find new entry
 *  @param key identifier of entry
 *  @return pointer to the entry or error if internal error occurs
 ***/
error( htab_pair_t_ptr ) htab_lookup_add(htab_t * t, htab_key_t key , sType type ){

    if(!t || !key){

        return_error( ERROR_HTAB_INVPTR , htab_pair_t_ptr );
    }
    if(!t->ptrs){
        
        return_error( ERROR_HTAB_INVBPTR , htab_pair_t_ptr );
    }
    unsigned long index = htab_hash_function(key) % t->arr_size;  
    
    struct htab_item * tmp = t->ptrs[index];
    struct htab_item * trailing = NULL;

    while(tmp){

        if(!strcmp(tmp->pair.key,key)){  // tmp->pair.key == key !

            tmp->pair.redefined = true; 
            return_value( &tmp->pair , htab_pair_t_ptr );
        }
        trailing = tmp;    
        tmp = tmp->next;
    }

    //struct htab_item * new_item = _create_new_item(key , type ); //creating new entry
    get_value( htab_item_ptr , new_item , _create_new_item( key , type) , htab_pair_t_ptr );
    /*if(!new_item){
        return_error( ERROR_MAL , htab_pair_t_ptr );
    }*/

    if(!trailing){        //creating connection

        t->ptrs[index] = new_item;
        
    } else {

        trailing->next = new_item;
    }
    t->size++;
    if(AVG_LEN_MAX < (t->arr_size / t->size)){   //avg size check
        htab_resize(t,2*t->arr_size);
    }
    return_value( &new_item->pair , htab_pair_t_ptr );
}