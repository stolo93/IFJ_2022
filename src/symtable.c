/****************************************************************
 * @name symtable.c
 * @author Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * @brief File contaning functions for hash table which used as symbol table in semantic analysis
 * @date 28.10.2022
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/

// NOLINTBEGIN (bugprone-suspicious-include)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/symtable.h"
#include "headers/interner.h"

#define AVG_LEN_MIN 40
#define AVG_LEN_MAX 150

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR( structFuncParam , structFuncParam );

/** Function which creates new intem for hash table identified by key
 *
 *  @param key identifier of entry
 *  @param type type of symbol which is being created
 *  @return pointer to the new item or error if internal error occurs
 ***/
error( htab_item_ptr ) _create_new_item(htab_key_t key , sType type ){

    struct htab_item* new_item = (struct htab_item* ) malloc(sizeof(struct htab_item));
    
    if(!new_item){
       
        return_error( ERROR_MAL , htab_item_ptr);
    }

    new_item->pair.key = (char* )key;                                   
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
        new_item->pair.diff.func.inParams = new_vec_structFuncParam();
    }
    new_item->next = NULL;

    return_value( new_item , htab_item_ptr );
}




/** Function which finds entry and raises redefined flag and returns pointer to it or if it fails it creates new entry.   
 *
 *  @param t table where we want increment or find new entry
 *  @param key identifier of entry
 *  @param type type of symbol if new entry will be created
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

        if( tmp->pair.key == key){  // tmp->pair.key == key !

            tmp->pair.redefined = true; 
            return_value( &tmp->pair , htab_pair_t_ptr );
        }
        trailing = tmp;    
        tmp = tmp->next;
    }

    get_value( htab_item_ptr , new_item , _create_new_item( key , type) , htab_pair_t_ptr );
    
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

/** Function which clears all records from table
 *
 *  @param t table from which we want to erase all records
 *  @return error if function was not successfull
 ***/
error(none) htab_clear(htab_t * t){

    if(!t){
        
        return_error(ERROR_HTAB_INVPTR, none);
    }

    for(size_t counter = 0; counter < t->arr_size; counter++){  

        struct htab_item * tmp = t->ptrs[counter];

        while(tmp){   
            
            struct htab_item * to_erase = tmp;
            tmp = tmp->next;
            
            if ( to_erase->pair.symType == function )
            {
                vec_structFuncParam_destroy( &to_erase->pair.diff.func.inParams );
            }
            free(to_erase);
        }
    }
    free(t->ptrs);   
    t->ptrs = NULL;
    t->size = 0;
    t->arr_size = 0;
    return_none();
}


/** Function for erasing one entry from table and changing size of table if neccesary
 *
 *  @param t table from which we want to erase entry
 *  @param key identifier of record
 *  @return succes of erasing or error if internal error occurs
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
        
            if ( tmp->pair.symType ==  function )
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

/** Function for finding record 
 *
 *  @param t table where we want to find entry
 *  @param key identifier of entry
 *  @return pointer to the entry, NULL pointer if entry don't exists or error if internal error occurs
 ***/
error(htab_pair_t_ptr) htab_find(htab_t * t, htab_key_t key){

    if(!t || !key){
        
        return_error( ERROR_HTAB_INVPTR , htab_pair_t_ptr);
    }
    if(!t->ptrs){
       
        return_error( ERROR_HTAB_INVBPTR , htab_pair_t_ptr);
    }

    size_t index = htab_hash_function(key) % t->arr_size; 
    
    struct htab_item * tmp = t->ptrs[index];

    while(tmp){

        if( tmp->pair.key == key ) {
            return_value( &tmp->pair , htab_pair_t_ptr );
        }
                
        tmp = tmp->next;
    }

    return_value( NULL , htab_pair_t_ptr );
}

/** Function which executes function on all entries from table
 *
 *  @param t table where we want execute function 
 *  @param f pointer to function which will be executed on all entries. This function can't alter any information in entry
 *  @return success of calling function or error if internal error occurs
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


/** Function which frees whole table 
 *
 *  @param t table which we want to free
 *  @return error if internal error occurs
 ***/
error(none ) htab_free(htab_t * t){

    if(!t){
        return_error( ERROR_HTAB_INVPTR , none );
    }
    htab_clear(t);  
    free(t);        
    return_none();
}

/** Hash function which is used by table
 *
 *  @param str key from entry
 *  @return Hash. Hash modulo arr_size will return index into hash table
 ***/
unsigned long  htab_hash_function(const char *str) {
          
    /*uint32_t h=0;     // must have 32 bits
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;*/

    unsigned long hash =  5381 ;
    int c;

    while( (c = (unsigned char)*str++ ))
    {
        hash = ((hash << 5) + hash ) + c;
    }

    return hash;
}

extern interner* interner_ptr; 

/** Function which interns names of buildin functions  
 *
 *  @param ptrs array where interned ptrs should be stored
 *  @return error if interning fails
 ***/
error( none) intern_functions( char* ptrs[])
{
    char strings[NUMBER_OF_BUILD_IN_STRINGS][ SIZE_OF_BUILD_IN_STRING ] = { "reads", "readi", "readf", "write",
                                                                            "strlen", "substring", "ord", "chr",
                                                                            "floatval", "intval", "strval",
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
    static const char* ptrs[NUMBER_OF_BUILD_IN_STRINGS];
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
            success = vec_structFuncParam_push_front( &(newEntry->diff.func.inParams) , param );
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
        else if( strcmp( newEntry->key , "floatval") == 0)
        {
            newEntry->diff.func.outType = floatingT;
        }
        else if( strcmp( newEntry->key , "intval") == 0)
        {
            newEntry->diff.func.outType = integerT;
        }
        else if( strcmp( newEntry->key , "strval") == 0)
        {
            newEntry->diff.func.outType = stringT;
        }
    }
    return_value( htab , htab_t_ptr );
}


/** Function which initialize hash table  
 *
 *  @param num number of lists which we want create
 *  @return pointer to the hash table or error if initialization fails
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

/** Function which will resize number of arrays with lists 
 *
 *  @param newn count of new lists 
 *  @param t table which we want to resize
 *  @return error if any internal error occurs
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
    //when resizing all entries must change their position so that all functions would function properly
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

/** Function which returns size of table 
 *
 *  @param t table whose size we want
 *  @return size of table or error if it is invalid pointer
 ***/
error(unsigned_long_long ) htab_size(const htab_t * t){

    if(!t){
        
        return_error( ERROR_HTAB_INVPTR , unsigned_long_long);
    }

    return_value( t->size , unsigned_long_long);
}

/** Function which returns bucket count of table 
 *
 *  @param t table whose bucket count we want
 *  @return bucket count of table or error if it is invalid pointer
 ***/
error(unsigned_long_long ) htab_bucket_count(const htab_t * t){

    if(!t){
        
        return_error( ERROR_HTAB_INVPTR , unsigned_long_long);
    }
    return_value( t->arr_size , unsigned_long_long );
}
// NOLINTEND (bugprone-suspicious-include)
