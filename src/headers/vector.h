/****************************************************************
 * @name vector.h
 * @author Adam Bezák <xbezak02@stud.fit.vutbr.cz>
 * @brief File containing macros for vector implementation
 * @date 20.10.2022
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#ifndef IFJ_2022_VECTOR_H
#define IFJ_2022_VECTOR_H

#include <stddef.h>
#include <stdlib.h>

#include "error.h"

#define BREZAKS_RANDOM_VECTOR_VER 2_1_0

#ifndef __assume
#define __assume(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)
#endif

// NOLINTBEGIN (bugprone-macro-parentheses)
#define DEFINE_VEC_PROTOTYPES(type, suffix)                                        \
typedef struct vec_ ## suffix {                                                    \
    type *  data;                                                                  \
    size_t len__;                                                                  \
    size_t capacity__;                                                             \
} vec_ ## suffix;                                                                  \
                                                                                   \
define_error(vec_ ## suffix)                                                       \
                                                                                   \
vec_ ## suffix new_vec_ ## suffix ();                                              \
error(vec_ ## suffix) new_vec_ ## suffix ## _with_capacity(size_t capacity);       \
error(none) vec_ ## suffix ## _push_front(vec_ ## suffix * vec, type item);        \
error(none) vec_ ## suffix ## _push_back(vec_ ## suffix * vec, type item);         \
error(type) vec_ ## suffix ## _pop_front(vec_ ## suffix * vec);                    \
error(type) vec_ ## suffix ## _pop_back(vec_ ## suffix * vec);                     \
error(type ## _ptr) vec_ ## suffix ## _get(vec_ ## suffix * vec, size_t index);    \
error(none) vec_ ## suffix ## _set(vec_ ## suffix * vec, size_t index, type item); \
error(none) vec_ ## suffix ## _remove(vec_ ## suffix * vec, size_t index);         \
error(none) vec_ ## suffix ##_insert(vec_ ##suffix * vec, size_t index, type item); \
size_t vec_ ## suffix ## _len(vec_ ## suffix * vec);                               \
size_t vec_ ## suffix ## _capacity(vec_ ## suffix * vec);                          \
void vec_ ## suffix ## _destroy (vec_ ## suffix * vec);                            \
error(none) vec_ ## suffix ## _resize (vec_ ## suffix  * vec, size_t new_size);    \
error(none) vec_ ## suffix ## _shrink_to_fit(vec_ ## suffix * vec);

// Assumes there's empty space in front of the used buffer
#define VEC_SHIFT_ELEMENTS_FORWARD(vec, suffix) for (size_t i = (vec)->len__; i > 0; i--) {(vec) -> data[i] = (vec) -> data[i - 1];}

#define VEC_SHIFT_ELEMENTS_BACKWARDS(vec, suffix) for (size_t i = 1; i < (vec)->len__; i++) {(vec) -> data[i - 1] = (vec) -> data[i];}

#define DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(type, suffix)                                \
vec_ ## suffix new_vec_ ## suffix () {                                                  \
    vec_ ## suffix empty = {NULL, 0, 0};                                                \
    return empty;                                                                       \
}                                                                                       \
                                                                                        \
error(vec_ ## suffix) new_vec_ ## suffix ## _with_capacity(size_t capacity) {           \
    vec_ ## suffix empty = new_vec_ ## suffix ();                                       \
    error(none) result = vec_ ## suffix ## _resize(&empty, capacity);                   \
    if (is_error(result)) {                                                             \
        forward_error(result, vec_ ## suffix);                                          \
    }                                                                                   \
                                                                                        \
    return_value(empty, vec_ ## suffix);                                                \
}                                                                                       \
                                                                                        \
error(type ## _ptr) vec_ ## suffix ## _get(vec_ ## suffix * vec, size_t index) {        \
    if (index >= vec->len__) {                                                          \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, type ## _ptr);                          \
    }                                                                                   \
                                                                                        \
    return_value(&(vec->data[index]), type ## _ptr);                                    \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _set(vec_ ## suffix * vec, size_t index, type item) {     \
    if (index >= vec->len__) {                                                          \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, none);                                  \
    }                                                                                   \
                                                                                        \
    vec->data[index] = item;                                                            \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ##_insert(vec_ ##suffix * vec, size_t index, type item)  {   \
    if (index > (vec)->len__) {                                                         \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, none);                                  \
    }                                                                                   \
                                                                                        \
    if (vec->len__ == 0) {                                                              \
        error(none) result = vec_ ## suffix ## _resize((vec) , 1);                      \
        if (is_error(result)) {                                                         \
            return result;                                                              \
        }                                                                               \
    } else if ((vec)->len__ >= (vec)->capacity__) {                                     \
        error(none) result = vec_ ## suffix ## _resize((vec) ,vec->capacity__ * 2);     \
        if (is_error(result)) {                                                         \
            return result;                                                              \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    for (size_t i = (vec)->len__; i > index; i--) {                                     \
        (vec) -> data[i] = (vec) -> data[i - 1];                                        \
    }                                                                                   \
                                                                                        \
    vec->data[index] = item;                                                            \
    vec->len__ += 1;                                                                    \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _remove(vec_ ## suffix * vec, size_t index) {             \
    if (index >= vec->len__) {                                                          \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, none);                                  \
    }                                                                                   \
                                                                                        \
    for (size_t i = index; i < vec-> len__ - 1; i++) {                                  \
        vec -> data[i] = vec -> data[i + 1];                                            \
    }                                                                                   \
    vec -> len__ -= 1;                                                                  \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _push_front(vec_ ## suffix * vec, type item) {            \
    if (vec -> capacity__ == 0) {                                                       \
        error(none) result = vec_ ## suffix ## _resize((vec) ,1);                       \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    } else if (vec -> capacity__ <= (vec -> len__)) {                                   \
        error(none) result = vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2); \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    }                                                                                   \
    VEC_SHIFT_ELEMENTS_FORWARD(vec, suffix)                                             \
    vec -> len__ += 1;                                                                  \
    vec -> data[0] = item;                                                              \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _push_back(vec_ ## suffix * vec, type item) {             \
    if (vec -> capacity__ == 0) {                                                       \
        error(none) result = vec_ ## suffix ## _resize((vec) ,1);                       \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    } else if (vec -> capacity__ <= (vec -> len__)) {                                   \
        error(none) result = vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2); \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    }                                                                                   \
    vec->data[vec -> len__] = item;                                                     \
    vec->len__ += 1;                                                                    \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(type) vec_ ## suffix ## _pop_front(vec_ ## suffix * vec) {                        \
    if (vec->len__ == 0) {                                                              \
        return_error(VECTOR_EMPTY_ERROR, type);                                         \
    }                                                                                   \
                                                                                        \
    type result_value = vec->data[0];                                                   \
    VEC_SHIFT_ELEMENTS_BACKWARDS(vec, suffix);                                          \
    vec -> len__ -= 1;                                                                  \
    return_value(result_value, type);                                                   \
}                                                                                       \
                                                                                        \
error(type) vec_ ## suffix ## _pop_back(vec_ ## suffix * vec) {                         \
    if (vec->len__ == 0) {                                                              \
        return_error(VECTOR_EMPTY_ERROR, type);                                         \
    }                                                                                   \
                                                                                        \
    vec -> len__ -= 1;                                                                  \
    type result_value = vec->data[vec->len__];                                          \
    return_value(result_value, type);                                                   \
}                                                                                       \
                                                                                        \
size_t vec_ ## suffix ## _len(vec_ ## suffix * vec) {return vec -> len__;}              \
                                                                                        \
size_t vec_ ## suffix ## _capacity(vec_ ## suffix * vec) {return vec -> capacity__;}    \
                                                                                        \
void vec_ ## suffix ## _destroy (vec_ ## suffix * vec) {                                \
    vec->len__ = 0;                                                                     \
    vec->capacity__ = 0;                                                                \
    free(vec->data);                                                                    \
    vec->data = NULL;                                                                   \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _resize (vec_ ## suffix * vec, size_t new_size) {         \
    void* new_data = realloc(vec->data, new_size * sizeof(type));                       \
    if (new_data == NULL) {                                                             \
        free(vec->data);                                                                \
        vec->len__ = 0;                                                                 \
        vec->capacity__ = 0;                                                            \
        vec->data = NULL;                                                               \
        return_error(VECTOR_REALLOC_ERROR, none);                                       \
    }                                                                                   \
    vec -> data = (type *)new_data;                                                     \
    vec -> capacity__ = new_size;                                                       \
                                                                                        \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _shrink_to_fit(vec_ ## suffix * vec) {                    \
    __assume(vec -> len__ <= vec-> capacity__);                                         \
    return vec_ ## suffix ## _resize(vec, vec -> len__);                                \
};

#define DEFINE_VEC_FUNCTIONS_WITH_DESTRUCTOR(type, suffix, destructor)                  \
vec_ ## suffix new_vec_ ## suffix () {                                                  \
    vec_ ## suffix empty = {NULL, 0, 0};                                                \
    return empty;                                                                       \
}                                                                                       \
                                                                                        \
error(vec_ ## suffix) new_vec_ ## suffix ## _with_capacity(size_t capacity) {           \
    vec_ ## suffix empty = new_vec_ ## suffix ();                                       \
    error(none) result = vec_ ## suffix ## _resize(&empty, sizeof(type) * capacity);    \
    if (is_error(result)) {                                                             \
        forward_error(result, vec_ ## suffix);                                          \
    }                                                                                   \
                                                                                        \
    return_value(empty, vec_ ## suffix);                                                \
}                                                                                       \
                                                                                        \
error(type ## _ptr) vec_ ## suffix ## _get(vec_ ## suffix * vec, size_t index) {        \
    if (index >= vec->len__) {                                                          \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, type ## _ptr);                          \
    }                                                                                   \
                                                                                        \
    return_value(&(vec->data[index]), type ## _ptr);                                    \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _set(vec_ ## suffix * vec, size_t index, type item) {     \
    if (index >= vec->len__) {                                                          \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, none);                                  \
    }                                                                                   \
                                                                                        \
    vec->data[index] = item;                                                            \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ##_insert(vec_ ##suffix * vec, size_t index, type item)  {   \
    if (index > (vec)->len__) {                                                         \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, none);                                  \
    }                                                                                   \
                                                                                        \
    if (vec->len__ == 0) {                                                              \
        error(none) result = vec_ ## suffix ## _resize((vec) , 1);                      \
        if (is_error(result)) {                                                         \
            return result;                                                              \
        }                                                                               \
    } else if ((vec)->len__ >= (vec)->capacity__) {                                     \
        error(none) result = vec_ ## suffix ## _resize((vec) ,vec->capacity__ * 2);     \
        if (is_error(result)) {                                                         \
            return result;                                                              \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    for (size_t i = (vec)->len__; i > index; i--) {                                     \
        (vec) -> data[i] = (vec) -> data[i - 1];                                        \
    }                                                                                   \
                                                                                        \
    vec->data[index] = item;                                                            \
    vec->len__ += 1;                                                                    \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _remove(vec_ ## suffix * vec, size_t index) {             \
    if (index >= vec->len__) {                                                          \
        return_error(VECTOR_INDEX_BOUNDS_ERROR, none);                                  \
    }                                                                                   \
    destructor(vec->data[index]);                                                       \
                                                                                        \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _push_front(vec_ ## suffix * vec, type item) {            \
    if (vec -> capacity__ == 0) {                                                       \
        error(none) result = vec_ ## suffix ## _resize((vec) , 1);                      \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    } else if (vec -> capacity__ <= (vec -> len__)) {                                   \
        error(none) result = vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2); \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    }                                                                                   \
    VEC_SHIFT_ELEMENTS_FORWARD(vec, suffix)                                             \
    vec -> len__ += 1;                                                                  \
    vec -> data[0] = item;                                                              \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _push_back(vec_ ## suffix * vec, type item) {             \
    if (vec -> capacity__ == 0) {                                                       \
        error(none) result = vec_ ## suffix ## _resize((vec) ,1);                       \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    } else if (vec -> capacity__ <= (vec -> len__)) {                                   \
        error(none) result = vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2); \
        if (is_error(result)) {                                                         \
            forward_error(result, none);                                                \
        }                                                                               \
    }                                                                                   \
    vec->data[vec -> len__] = item;                                                     \
    vec->len__ += 1;                                                                    \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(type) vec_ ## suffix ## _pop_front(vec_ ## suffix * vec) {                        \
    if (vec->len__ == 0) {                                                              \
        return_error(VECTOR_EMPTY_ERROR, type);                                         \
    }                                                                                   \
                                                                                        \
    type result_value = vec->data[0];                                                   \
    VEC_SHIFT_ELEMENTS_BACKWARDS(vec, suffix);                                          \
    vec -> len__ -= 1;                                                                  \
    return_value(result_value, type);                                                   \
}                                                                                       \
                                                                                        \
error(type) vec_ ## suffix ## _pop_back(vec_ ## suffix * vec) {                         \
    if (vec->len__ == 0) {                                                              \
        return_error(VECTOR_EMPTY_ERROR, type);                                         \
    }                                                                                   \
                                                                                        \
    vec -> len__ -= 1;                                                                  \
    type result_value = vec->data[vec->len__];                                          \
    return_value(result_value, type);                                                   \
}                                                                                       \
                                                                                        \
size_t vec_ ## suffix ## _len(vec_ ## suffix * vec) {return vec -> len__;}              \
                                                                                        \
size_t vec_ ## suffix ## _capacity(vec_ ## suffix * vec) {return vec -> capacity__;}    \
                                                                                        \
void vec_ ## suffix ## _destroy (vec_ ## suffix * vec) {                                \
    for (size_t i = 0; i < vec -> len__; i++) {                                         \
        destructor(vec -> data[i]);                                                     \
    }                                                                                   \
    vec->len__ = 0;                                                                     \
    vec->capacity__ = 0;                                                                \
    free(vec -> data);                                                                  \
    vec->data = NULL;                                                                   \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _resize (vec_ ## suffix * vec, size_t new_size) {         \
    if (new_size > vec -> capacity__) {                                                 \
        void* new_data = realloc(vec->data, new_size * sizeof(type));                   \
        if (new_data == NULL) {                                                         \
            free(vec->data);                                                            \
            vec->len__ = 0;                                                             \
            vec->capacity__ = 0;                                                        \
            vec->data = NULL;                                                           \
            return_error(VECTOR_REALLOC_ERROR, none);                                   \
        }                                                                               \
                                                                                        \
        vec -> data = (type *)new_data;                                                 \
    } else if (new_size < vec -> capacity__) {                                          \
        for (size_t i = vec -> len__; i > new_size; i--) {                              \
            destructor(vec->data[i - 1]);                                               \
        }                                                                               \
        void* new_data = realloc(vec->data, new_size * sizeof(type));                   \
        if (new_data == NULL) {                                                         \
            free(vec->data);                                                            \
            vec->len__ = 0;                                                             \
            vec->capacity__ = 0;                                                        \
            vec->data = NULL;                                                           \
            return_error(VECTOR_REALLOC_ERROR, none);                                   \
        }                                                                               \
                                                                                        \
        vec -> data = (type*)new_data;                                                  \
    }                                                                                   \
                                                                                        \
    vec -> capacity__ = new_size;                                                       \
    return_none();                                                                      \
}                                                                                       \
                                                                                        \
error(none) vec_ ## suffix ## _shrink_to_fit(vec_ ## suffix * vec) {                    \
    __assume(vec -> len__ <= vec-> capacity__);                                         \
    return vec_ ## suffix ## _resize(vec, vec -> len__);                                \
};
// NOLINTEND (bugprone-macro-parentheses)

#endif //IFJ_2022_VECTOR_H
