#include "error.h"

typedef long long long_long;
typedef unsigned char unsigned_char;
typedef unsigned short unsigned_short;
typedef unsigned int unsigned_int;
typedef unsigned long unsigned_long;
typedef unsigned long long unsigned_long_long;

define_error(char)
define_error(unsigned_char)

define_error(short)
define_error(unsigned_short)

define_error(int)
define_error(unsigned_int)

define_error(long)
define_error(unsigned_long)

define_error(long_long)
define_error(unsigned_long_long)

define_error(float)
define_error(double)

define_error(none)