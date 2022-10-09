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

typedef enum dataType { integer , floating , string , notDefined , noType } dType ;
typedef enum symbolType { function , variable } sType ;

// Tabulka:
struct htab;    // neúplná deklarace struktury - uživatel nevidí obsah
typedef struct htab htab_t;     // typedef podle zadání

// Typy:
typedef const char * htab_key_t;        // typ klíče
typedef int htab_value_t;               // typ hodnoty

// Dvojice dat v tabulce:
typedef struct htab_pair {
    dType type ;          //data type variable
    char * key ;         //name of symbol
    union information     //what is inside variable
    {
        char* string ;
        int integer ;
        double decNumber ;
    } info ;
    sType symType;    //type of symbol
    int val;      
} htab_pair_t;                 // typedef podle zadání



size_t htab_hash_function(htab_key_t str);


htab_t *htab_init(size_t n);                    
size_t htab_size(const htab_t * t);             
size_t htab_bucket_count(const htab_t * t);     
void htab_resize(htab_t *t, size_t newn);       
                                                

htab_pair_t * htab_find(htab_t * t, htab_key_t key); 
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key);

bool htab_erase(htab_t * t, char* key);    



void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data));

bool htab_clear(htab_t * t);    
void htab_free(htab_t * t);     
#endif // __HTAB_H__