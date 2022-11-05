#include "headers/IFJ_2022.h"
#include "headers/vector.h"

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(char, char);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_char, unsigned_char);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(short, short);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_short, unsigned_short);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(int, int);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_int, unsigned_int);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(long, long)
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_long, unsigned_long);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(long_long, long_long);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(unsigned_long_long, unsigned_long_long);

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(float, float);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(double, double);

DEFINE_VEC_FUNCTIONS_WITH_DESTRUCTOR(token_ptr, token_ptr, free);
