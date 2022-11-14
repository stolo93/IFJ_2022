#ifndef IFJ_2022_INTERNER_H
#define IFJ_2022_INTERNER_H

#include "error.h"
#include "IFJ_2022.h"
#include "stddef.h"
#include "vector.h"
#include "vector_ord.h"

typedef const char* string_t;
typedef string_t* string_t_ptr;

define_error(string_t);
define_error(string_t_ptr);
DEFINE_VEC_PROTOTYPES(string_t, string);

typedef struct interner {
    vec_string strings;
} interner;

typedef const char* intern_id;

define_error(interner);
define_error(intern_id);

interner interner_new();
error(interner) interner_new_with_capacity(size_t capacity);
error(intern_id) intern(interner* interner_obj, char* string);
error(intern_id) intern_copy(interner* interner_obj, const char* string);
bool is_interned(interner* interner_obj, const char* string);
const char* get_string(interner* interner_obj, intern_id id);
void interner_destroy(interner* interner_obj);


#endif //IFJ_2022_INTERNER_H
