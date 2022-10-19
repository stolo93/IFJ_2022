#ifndef IFJ_2022_IFJ_2022_H
#define IFJ_2022_IFJ_2022_H

#include "error.h"
#include "vector.h"

typedef long long long_long;
typedef unsigned char unsigned_char;
typedef unsigned short unsigned_short;
typedef unsigned int unsigned_int;
typedef unsigned long unsigned_long;
typedef unsigned long long unsigned_long_long;

typedef char* char_ptr;
typedef unsigned char * unsigned_char_ptr;
typedef short* short_ptr;
typedef unsigned short* unsigned_short_ptr;
typedef int* int_ptr;
typedef unsigned int * unsigned_int_ptr;
typedef long* long_ptr;
typedef unsigned long * unsigned_long_ptr;
typedef long long* long_long_ptr;
typedef unsigned long long * unsigned_long_long_ptr;
typedef float* float_ptr;
typedef double* double_ptr;

define_error(char)
define_error(unsigned_char)
define_error(char_ptr)
define_error(unsigned_char_ptr)

define_error(short)
define_error(unsigned_short)
define_error(short_ptr)
define_error(unsigned_short_ptr)

define_error(int)
define_error(unsigned_int)
define_error(int_ptr)
define_error(unsigned_int_ptr)

define_error(long)
define_error(unsigned_long)
define_error(long_ptr)
define_error(unsigned_long_ptr)

define_error(long_long)
define_error(unsigned_long_long)
define_error(long_long_ptr)
define_error(unsigned_long_long_ptr)

define_error(float)
define_error(double)
define_error(float_ptr)
define_error(double_ptr)

define_error(none)

DEFINE_VEC_PROTOTYPES(char, char);
DEFINE_VEC_PROTOTYPES(unsigned_char, unsigned_char);

DEFINE_VEC_PROTOTYPES(short, short);
DEFINE_VEC_PROTOTYPES(unsigned_short, unsigned_short);

DEFINE_VEC_PROTOTYPES(int, int);
DEFINE_VEC_PROTOTYPES(unsigned_int, unsigned_int);

DEFINE_VEC_PROTOTYPES(long, long)
DEFINE_VEC_PROTOTYPES(unsigned_long, unsigned_long);

DEFINE_VEC_PROTOTYPES(long_long, long_long);
DEFINE_VEC_PROTOTYPES(unsigned_long_long, unsigned_long_long);

DEFINE_VEC_PROTOTYPES(float, float);
DEFINE_VEC_PROTOTYPES(double, double);




//enum of all token types even internal , do NOT change order of elements between def and identOfFunct
typedef enum tokenType { def , elseT, /*floatT ,*/ functionT , ifT , /*intT ,*/ nullT , returnT , 
             /*stringT ,*/ voidT , whileT , identOfFunct , identOfVar , identOfType , 
             integer , decNum , string , lineComment , multiLineComm ,
             plusSign, minusSign , multiply , division , concatenation ,
             lessOper , lessOrEqOper , moreOper , moreOrEqOper ,
             EqOper , notEqOper, assigment , openParen , closeParen , openSetParen , 
             closeSetParen , comma , semicolon , colon , endOfFile , identOfTypeN ,
             multiLineCommPE, prolog } tokenType;

typedef struct token
{
    tokenType discriminant;
    union information 
    {
        int integer;
        double decNuber;
        char* string;
    } info;
} token_t ;

typedef token_t* token_ptr;

define_error(token_ptr);

#endif // IFJ_2022_IFJ_2022_H