/****************************************************************
 * @name htab_struct.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include "htab.h"

struct htab{
    size_t size;
    size_t arr_size;
    struct htab_item** ptrs;
};

struct htab_item{
    htab_pair_t pair;
    struct htab_item* next;
};