/****************************************************************
 * @name vector.c
 * @author Adam Bezák <xbezak02@stud.fit.vutbr.cz>
 * @brief File contaning declarations of functions for vector implementation
 * @date 20.10.2022
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include "headers/IFJ_2022.h"
#include "headers/vector.h"
#include "headers/vector_ord.h"

enum VEC_ORDERING STRING_CMP(const char* a, const char* b) {
    int comparison = strcmp(a, b);
    if (comparison < 0) {
        return LESS_VEC;
    }

    if (comparison > 0) {
        return GREATER_VEC;
    }

    return EQUAL_VEC;
}

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(char, char);
DEFINE_VEC_ORD_FUNCTIONS(char, char, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_char, unsigned_char);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_char, unsigned_char, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(short, short);
DEFINE_VEC_ORD_FUNCTIONS(short, short, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_short, unsigned_short);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_short, unsigned_short, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(int, int);
DEFINE_VEC_ORD_FUNCTIONS(int, int, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_int, unsigned_int);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_int, unsigned_int, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(long, long);
DEFINE_VEC_ORD_FUNCTIONS(long, long, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_long, unsigned_long);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_long, unsigned_long, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(long_long, long_long);
DEFINE_VEC_ORD_FUNCTIONS(long_long, long_long, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_long_long, unsigned_long_long);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_long_long, unsigned_long_long, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(float, float);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(double, double);

DEFINE_VEC_FUNCTIONS_WITH_DESTRUCTOR(token_ptr, token_ptr, free);

