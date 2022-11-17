#ifndef IFJ_2022_VECTOR_ORD_H
#define IFJ_2022_VECTOR_ORD_H

#include <string.h>
#include "vector.h"
#include "IFJ_2022.h"

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE (size_t)64
#endif

// Using vec suffix so we don't alias common verbs
enum VEC_ORDERING {
    LESS_VEC,
    EQUAL_VEC,
    GREATER_VEC,
};

// NOLINTBEGIN (bugprone-macro-parentheses)

#define SIMPLE_CMP(a, b) (((a) < (b)) ? LESS_VEC : ((a) > (b)) ? GREATER_VEC : EQUAL_VEC)
#define ORD_REVERSE(ord) ((ord) == LESS_VEC) ? GREATER_VEC : ((ord) == GREATER_VEC) ? LESS_VEC : EQUAL_VEC

enum VEC_ORDERING STRING_CMP(const char* a, const char* b);

#define DEFINE_VEC_ORD_PROTOTYPES(type, suffix)                                \
void vec_ ## suffix ## _sort(vec_ ## suffix * vec);                            \
void vec_ ## suffix ## _sort_unstable(vec_ ## suffix * vec);                   \
bool vec_ ## suffix ## _is_sorted(const vec_ ## suffix * vec);                 \
typeof(type*) vec_ ## suffix ## _find(const vec_ ## suffix * vec, typeof(type) item);          \
typeof(type*) vec_ ## suffix ## _find_sorted(const vec_ ## suffix * vec, typeof(type) item);   \
error(none) vec_ ## suffix ## _insert_sorted(vec_ ## suffix * vec, typeof(type) item); \

// ORD_FUNCTION takes two instances of a vec value and returns VEC_ORDERING
// Sorts are adaptive and switch to insertion sort for small enough sequences
#define DEFINE_VEC_ORD_FUNCTIONS(type, suffix, ORD_FUNCTION)                           \
bool vec_ ## suffix ## _is_sorted(const vec_ ## suffix * vec) {                        \
    for (size_t i = 1; i < vec -> len__; i ++) {                                       \
        if (ORD_FUNCTION(vec->data[i - 1], vec->data[i]) == GREATER_VEC) return false;    \
   }                                                                                   \
                                                                                       \
   return true;                                                                        \
}                                                                                      \
                                                                                       \
void vec_ ## suffix ## _insertion_sort(vec_ ## suffix * vec) {                         \
    for (size_t i = 1; i < (vec)->len__; i++) {                                        \
        typeof(type) comparator = vec->data[i];                                        \
        size_t j = i - 1;                                                              \
        while (ORD_FUNCTION(vec->data[j], comparator) == GREATER_VEC){                 \
            vec->data[j + 1] = vec->data[j];                                           \
            if (j == 0) {                                                              \
                j--;                                                                   \
                break;                                                                 \
            }                                                                          \
            j--;                                                                       \
        }                                                                              \
        vec->data[j + 1] = comparator;                                                 \
    }                                                                                  \
}                                                                                      \
                                                                                       \
void vec_ ## suffix ## _sort(vec_ ## suffix * vec) {                                   \
    vec_ ## suffix ## _insertion_sort(vec);                                            \
}                                                                                      \
                                                                                       \
size_t vec_ ## suffix ## _partition(vec_ ## suffix * vec) {                            \
    typeof(type) pivot = vec->data[vec->len__ - 1];                                    \
                                                                                       \
    size_t smaller = -1;                                                               \
                                                                                       \
    for (size_t larger = 0; larger <= vec->len__ - 2; larger++) {                      \
        if (ORD_FUNCTION(vec->data[larger], pivot) == LESS_VEC) {                      \
            smaller++;                                                                 \
            typeof(type) temp = vec->data[smaller];                                    \
            vec->data[smaller] = vec->data[larger];                                    \
            vec->data[larger] = temp;                                                  \
        }                                                                              \
    }                                                                                  \
                                                                                       \
    typeof(type) temp = vec->data[smaller + 1];                                        \
    vec->data[smaller + 1] = vec->data[vec->len__ - 1];                                \
    vec->data[vec->len__ - 1] = temp;                                                  \
    return smaller + 1;                                                                \
}                                                                                      \
                                                                                       \
void vec_ ## suffix ## _sort_unstable(vec_ ## suffix * vec) {                          \
    if (vec->len__ < 2) {                                                              \
        return;                                                                        \
    }                                                                                  \
                                                                                       \
    if (sizeof(type) * vec->len__ <= (CACHE_LINE_SIZE * 2)) {                          \
        vec_ ## suffix ## _insertion_sort(vec);                                        \
        return;                                                                        \
    }                                                                                  \
                                                                                       \
    size_t partition_point = vec_ ## suffix ## _partition(vec);                        \
                                                                                       \
    if (partition_point > 1) {                                                         \
        vec_ ## suffix left_partition = {                                              \
                .len__ = partition_point,                                              \
                .capacity__ = partition_point,                                         \
                .data = vec->data                                                      \
        };                                                                             \
        vec_ ## suffix ## _sort_unstable(&left_partition);                             \
    }                                                                                  \
                                                                                       \
    if (partition_point < vec->len__ - 2) {                                            \
        vec_ ## suffix right_partition = {                                             \
                .len__ = (vec->len__ - partition_point) - 1,                           \
                .capacity__ = (vec->len__ - partition_point) - 1,                      \
                .data = vec->data + partition_point + 1                                \
        };                                                                             \
        vec_ ## suffix ## _sort_unstable(&right_partition);                            \
    }                                                                                  \
}                                                                                      \
                                                                                       \
typeof(type*) vec_ ## suffix ## _find(const vec_ ## suffix * vec, typeof(type) item) { \
    for (size_t i = 0; i < vec->len__; ++i) {                                          \
        if (ORD_FUNCTION(vec->data[i], item) == EQUAL_VEC) {                           \
            return vec->data + i;                                                      \
        }                                                                              \
    }                                                                                  \
                                                                                       \
    return NULL;                                                                       \
}                                                                                      \
                                                                                       \
size_t vec_ ## suffix ## _binary_search(const vec_ ## suffix*vec, typeof(type) item) { \
    size_t left = 0;                                                                   \
    size_t right = vec->len__;                                                         \
                                                                                       \
    while (left < right) {                                                             \
        size_t middle = (left + right) / 2;                                            \
        if (ORD_FUNCTION(vec->data[middle], item) == LESS_VEC) {                       \
            left = middle + 1;                                                         \
        } else {                                                                       \
            right = middle;                                                            \
        }                                                                              \
    }                                                                                  \
                                                                                       \
    return left;                                                                       \
}                                                                                      \
                                                                                       \
typeof(type*) vec_ ## suffix ## _find_sorted(const vec_ ## suffix*vec, typeof(type) item) {  \
    size_t location = vec_ ## suffix ## _binary_search(vec, item);                     \
                                                                                       \
    if (ORD_FUNCTION(vec->data[location], item) == EQUAL_VEC) {                        \
        return vec->data + location;                                                   \
    }                                                                                  \
                                                                                       \
    return NULL;                                                                       \
}                                                                                      \
                                                                                       \
error(none) vec_ ## suffix ## _insert_sorted(vec_ ## suffix * vec, typeof(type) item) {        \
    if ((vec)->len__ >= (vec)->capacity__) {                                           \
        error(none) result = vec_ ## suffix ## _resize((vec) ,vec->capacity__);        \
        if (is_error(result)) {                                                        \
            return result;                                                             \
        }                                                                              \
    }                                                                                  \
                                                                                       \
    size_t location = vec_ ## suffix ## _binary_search(vec, item);                     \
    vec_ ## suffix ## _insert(vec, location, item);                                    \
    return_none();                                                                     \
}                                                                                      \

// NOLINTEND (bugprone-macro-parentheses)

#endif //IFJ_2022_VECTOR_ORD_H
