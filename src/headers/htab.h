/****************************************************************
 * @name htab.h
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#ifndef __HTAB_H__
#define __HTAB_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include "error.h"
#include "IFJ_2022.h"

typedef enum dataType { integerT , floatingT , stringT , notDefined , noType } dType ;
typedef enum symbolType { function , variable } sType ;

// Table
struct htab;    // user cann't access this struct
typedef struct htab htab_t;     

// Typy:
typedef const char * htab_key_t;        // key type
typedef int htab_value_t;

typedef struct struct_var
{
    dType dataType;
    union dInformation     //what is inside variable
    {
        char* string ;
        int integer ;
        double  floating;
        
    } info ;   
} structVar;

typedef struct struct_func
{
    dType outType;
    //TODO vector of in variables
} structFunc;

// Dvojice dat v tabulce:
typedef struct htab_pair {
    
    sType symType;    //type of symbol
    char * key ;         //name of symbol
    union tInformation
    {
        structVar var;
        structFunc func;
        
    } diff;
     
} htab_pair_t;                 

typedef htab_pair_t* htab_pair_t_ptr;
typedef htab_t* htab_t_ptr; 

define_error(htab_pair_t_ptr);
define_error( htab_t_ptr );
define_error(_Bool ); //must use _Bool because otherwise it woudln't work

unsigned long htab_hash_function(htab_key_t str);


error( htab_t_ptr ) htab_init(size_t n);                    
error(unsigned_long_long ) htab_size(const htab_t * t);             
error(unsigned_long_long ) htab_bucket_count(const htab_t * t);     
error(none ) htab_resize(htab_t *t, size_t newn);       
                                                

error(htab_pair_t_ptr) htab_find(htab_t * t, htab_key_t key); 
error(htab_pair_t_ptr) htab_lookup_add(htab_t * t, htab_key_t key , sType type);

error(_Bool ) htab_erase(htab_t * t, char* key);    


error(_Bool ) htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data));

error(none) htab_clear(htab_t * t);    
error(none) htab_free(htab_t * t);     
#endif // __HTAB_H__