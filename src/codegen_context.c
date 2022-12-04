/****************************************************************
 * @name : codegen_context.c
 * @author : Adam Bezák <xbezak02@stud.fit.vutbr.cz>
 * @brief : A file containing functions a data structures needed to keep track variable lifetimes
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/

#include "headers/codegen_context.h"

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(char_ptr, context);
DEFINE_VEC_ORD_FUNCTIONS(char_ptr, context, SIMPLE_CMP);

error(vec_context) clone_context_vec(vec_context* src) {
    vec_context new = new_vec_context();

    error(none) result = vec_context_resize(&new, vec_context_len(src));
    if (is_error(result)) {
        forward_error(result, vec_context);
    }

    new.len__ = src->len__;
    memcpy(new.data, src->data, sizeof(char_ptr) * vec_context_len(src));

    return_value(new, vec_context);
}