#include <stddef.h>

#include "headers/interner.h"

#define interner_error_reducer(string) free((void*)string);
DEFINE_VEC_FUNCTIONS_WITH_DESTRUCTOR(string_t, string, interner_error_reducer);
DEFINE_VEC_ORD_FUNCTIONS(string_t, string, STRING_CMP);

interner interner_new() {
    interner new = {.strings = new_vec_string()};
    return new;
}

error(interner) interner_new_with_capacity(size_t capacity) {
    get_value(vec_string, large_vec, new_vec_string_with_capacity(capacity), interner);
    interner new = {.strings = large_vec};
    return_value(new, interner);
}

error(intern_id) intern(interner* interner_obj, char* string) {
    size_t index = vec_string_binary_search(&(interner_obj->strings), string);
    if (index == vec_string_len(&(interner_obj->strings))) {
        vec_string_push_back(&(interner_obj->strings), string);
        return_value(string, intern_id);
    } else if (strcmp(string, interner_obj->strings.data[index]) == 0) {
        free((void*)string);
        return_value(interner_obj->strings.data[index], intern_id);
    } else {
        vec_string_insert(&(interner_obj->strings), index, string);
        return_value(string, intern_id);
    }
}

error(intern_id) intern_copy(interner* interner_obj, const char* string) {

    size_t string_len = strlen(string) + 1;
    char* new_string = malloc(string_len * sizeof(char));
    if (new_string == NULL) {
        return_error(ERROR_MAL, intern_id);
    }

    memcpy(new_string, string, string_len);

    return intern(interner_obj, new_string);
}


bool is_interned(interner* interner_obj, const char* string) {
    return vec_string_find_sorted(&(interner_obj->strings), string) != NULL;
}

const char* get_string(interner* interner_obj, intern_id id) {
    for (size_t i = 0; i < vec_string_len(&(interner_obj->strings)); i++) {
        if (id == interner_obj->strings.data[i]) {
            return id;
        }
    }

    return NULL;
}

void interner_destroy(interner* interner_obj) {
    vec_string_destroy(&(interner_obj->strings));
}