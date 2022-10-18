//
// Created by brezak on 10/18/22.
//

#ifndef IFJ_2022_VECTOR_H
#define IFJ_2022_VECTOR_H

#include <stddef.h>
#include <stdlib.h>

#define BREZAKS_RANDOM_VECTOR_VER 1_0_0

#define DEFINE_VEC_PROTOTYPES(type, suffix)                                 \
typedef struct vec_ ## suffix {                                             \
    type *  data;                                                           \
    size_t len__;                                                           \
    size_t capacity__;                                                      \
} vec_ ## suffix;                                                           \
                                                                            \
vec_ ## suffix new_vec_ ## suffix ();                                       \
vec_ ## suffix new_vec_ ## suffix ## _with_capacity(size_t capacity);       \
void vec_ ## suffix ## _push_front(vec_ ## suffix * vec, type item);        \
void vec_ ## suffix ## _push_back(vec_ ## suffix * vec, type item);         \
type vec_ ## suffix ## _pop_front(vec_ ## suffix * vec);                    \
type vec_ ## suffix ## _pop_back(vec_ ## suffix * vec);                     \
type * vec_ ## suffix ## _get(vec_ ## suffix * vec, size_t index);          \
void vec_ ## suffix ## _set(vec_ ## suffix * vec, size_t index, type item); \
void vec_ ## suffix ## _remove(vec_ ## suffix * vec, size_t index);         \
size_t vec_ ## suffix ## _len(vec_ ## suffix * vec);                        \
size_t vec_ ## suffix ## _capacity(vec_ ## suffix * vec);                   \
void vec_ ## suffix ## _destroy (vec_ ## suffix * vec);                     \
void vec_ ## suffix ## _resize (vec_ ## suffix  * vec, size_t new_size);

#define VEC_SHIFT_ELEMENTS_FORWARD(vec, suffix) for (size_t i = (vec) -> len__ + 1; i > 1; i--) {(vec) -> data[i] = (vec) -> data[i - 1];}
#define VEC_SHIFT_ELEMENTS_BACKWARDS(vec, suffix) for (size_t i = (vec) -> len__ + 1; i > 1; i--) {(vec) -> data[i - 1] = (vec) -> data[i];}

#define DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(type, suffix)                                  \
vec_ ## suffix new_vec_ ## suffix () {                                                    \
    vec_ ## suffix empty = {NULL, 0, 0};                                                  \
    return empty;                                                                         \
}                                                                                         \
                                                                                          \
vec_ ## suffix new_vec_ ## suffix ## _with_capacity(size_t capacity) {                    \
    vec_ ## suffix empty = new_vec_ ## suffix ();                                         \
    vec_ ## suffix ## _resize(&empty, sizeof(type) * capacity);                           \
    return empty;                                                                         \
}                                                                                         \
                                                                                          \
type * vec_ ## suffix ## _get(vec_ ## suffix * vec, size_t index) {                       \
    return &(vec->data[index]);                                                           \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _set(vec_ ## suffix * vec, size_t index, type item) {              \
    vec->data[index] = item;                                                              \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _remove(vec_ ## suffix * vec, size_t index) {                      \
    for (size_t i = index; i < vec-> len__ - 1; i++) {                                    \
        vec -> data[i] = vec -> data[i + 1];                                              \
    }                                                                                     \
    vec -> len__ -= 1;                                                                    \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _push_front(vec_ ## suffix * vec, type item) {                     \
    if (vec -> capacity__ == (vec -> len__)) {                                            \
        vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2);                        \
    }                                                                                     \
    VEC_SHIFT_ELEMENTS_FORWARD(vec, suffix)                                               \
    vec -> len__ += 1;                                                                    \
    vec -> data[0] = item;                                                                \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _push_back(vec_ ## suffix * vec, type item) {                      \
    if (vec -> capacity__ == (vec -> len__)) {                                            \
        vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2);                        \
    }                                                                                     \
    vec->len__ += 1;                                                                      \
    vec->data[vec -> len__] = item;                                                       \
}                                                                                         \
                                                                                          \
type vec_ ## suffix ## _pop_front(vec_ ## suffix * vec) {                                 \
    type return_value = vec->data[0];                                                     \
    VEC_SHIFT_ELEMENTS_BACKWARDS(vec, suffix);                                            \
    vec -> len__ -= 1;                                                                    \
    return return_value;                                                                  \
}                                                                                         \
                                                                                          \
type vec_ ## suffix ## _pop_back(vec_ ## suffix * vec) {                                  \
    type return_value = vec->data[vec->len__];                                            \
    vec -> len__ -= 1;                                                                    \
    return return_value;                                                                  \
}                                                                                         \
                                                                                          \
size_t vec_ ## suffix ## _len(vec_ ## suffix * vec) {return vec -> len__;}                \
                                                                                          \
size_t vec_ ## suffix ## _capacity(vec_ ## suffix * vec) {return vec -> capacity__;}      \
                                                                                          \
void vec_ ## suffix ## _destroy (vec_ ## suffix * vec) {free(vec->data);}                 \
                                                                                          \
void vec_ ## suffix ## _resize (vec_ ## suffix * vec, size_t new_size) {                  \
    void* new_data = realloc(vec->data, new_size * sizeof(type));                         \
    if (new_data == NULL) {return;}                                                       \
    vec -> data = (type *)new_data;                                                       \
}

#define DEFINE_VEC_FUNCTIONS_WITH_DESTRUCTOR(type, suffix, destructor)                    \
vec_ ## suffix new_vec_ ## suffix () {                                                    \
    vec_ ## suffix empty = {NULL, 0, 0};                                                  \
    return empty;                                                                         \
}                                                                                         \
                                                                                          \
vec_ ## suffix new_vec_ ## suffix ## _with_capacity(size_t capacity) {                    \
    vec_ ## suffix empty = new_vec_ ## suffix ();                                         \
    vec_ ## suffix ## _resize(&empty, sizeof(type) * capacity);                           \
    return empty;                                                                         \
}                                                                                         \
                                                                                          \
type * vec_ ## suffix ## _get(vec_ ## suffix * vec, size_t index) {                       \
    return &(vec->data[index]);                                                           \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _set(vec_ ## suffix * vec, size_t index, type item) {              \
    vec->data[index] = item;                                                              \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _remove(vec_ ## suffix * vec, size_t index) {                      \
    destructor(vec->data[index]);                                                         \
    for (size_t i = index; i < vec-> len__ - 1; i++) {                                    \
        vec -> data[i] = vec -> data[i + 1];                                              \
    }                                                                                     \
    vec -> len__ -= 1;                                                                    \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _push_front(vec_ ## suffix * vec, type item) {                     \
    if (vec -> capacity__ == (vec -> len__)) {                                            \
        vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2);                        \
    }                                                                                     \
    VEC_SHIFT_ELEMENTS_FORWARD(vec, suffix)                                               \
    vec -> len__ += 1;                                                                    \
    vec -> data[0] = item;                                                                \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _push_back(vec_ ## suffix * vec, type item) {                      \
    if (vec -> capacity__ == (vec -> len__)) {                                            \
        vec_ ## suffix ## _resize((vec) ,(vec) -> capacity__ * 2);                        \
    }                                                                                     \
    vec->len__ += 1;                                                                      \
    vec->data[vec -> len__] = item;                                                       \
}                                                                                         \
                                                                                          \
type vec_ ## suffix ## _pop_front(vec_ ## suffix * vec) {                                 \
    type return_value = vec->data[0];                                                     \
    VEC_SHIFT_ELEMENTS_BACKWARDS(vec, suffix);                                            \
    vec -> len__ -= 1;                                                                    \
    return return_value;                                                                  \
}                                                                                         \
                                                                                          \
type vec_ ## suffix ## _pop_back(vec_ ## suffix * vec) {                                  \
    type return_value = vec->data[vec->len__];                                            \
    vec -> len__ -= 1;                                                                    \
    return return_value;                                                                  \
}                                                                                         \
                                                                                          \
size_t vec_ ## suffix ## _len(vec_ ## suffix * vec) {return vec -> len__;}                \
                                                                                          \
size_t vec_ ## suffix ## _capacity(vec_ ## suffix * vec) {return vec -> capacity__;}      \
                                                                                          \
void vec_ ## suffix ## _destroy (vec_ ## suffix * vec) {                                  \
    for (size_t i = 0; i < vec -> len__; i++) {                                           \
        destructor(vec -> data[i]);                                                       \
    }                                                                                     \
    free(vec -> data);                                                                    \
}                                                                                         \
                                                                                          \
void vec_ ## suffix ## _resize (vec_ ## suffix * vec, size_t new_size) {                  \
    if (new_size > vec -> capacity__) {                                                   \
        void* new_data = realloc(vec->data, new_size * sizeof(type));                     \
        if (new_data == NULL) {                                                           \
            vec->len__ == 0;                                                              \
            vec->capacity__ == 0;                                                         \
            vec->data = NULL;                                                             \
            return;                                                                       \
        }                                                                                 \
                                                                                          \
        vec -> data = (type *)new_data;                                                   \
    } else if (new_size < vec -> capacity__) {                                            \
        size_t shift_amount = vec->capacity__ - new_size;                                 \
        for (size_t i = 0; i < shift_amount; i++) {                                       \
            destructor(vec->data[i]);                                                     \
        };                                                                                \
        for (size_t i = 0; i < vec->capacity__ - shift_amount; i++) {                     \
            vec->data[i] = vec->data[i + shift_amount];                                   \
        }                                                                                 \
    }                                                                                     \
}                                                                                         \

#endif //IFJ_2022_VECTOR_H
