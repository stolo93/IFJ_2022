#include "headers/IFJ_2022.h"
#include "headers/vector.h"
#include "headers/vector_ord.h"

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

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(long, long)
DEFINE_VEC_ORD_FUNCTIONS(long, long, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_long, unsigned_long);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_long, unsigned_long, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(long_long, long_long);
DEFINE_VEC_ORD_FUNCTIONS(long_long, long_long, SIMPLE_CMP);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_long_long, unsigned_long_long);
DEFINE_VEC_ORD_FUNCTIONS(unsigned_long_long, unsigned_long_long, SIMPLE_CMP);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(float, float);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(double, double);