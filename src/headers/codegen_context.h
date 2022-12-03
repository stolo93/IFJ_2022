/****************************************************************
 * @name codegen_context.h
 * @author : Adam Bezák <xbezak02@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/

#ifndef IFJ_2022_CODEGEN_CONTEXT_H
#define IFJ_2022_CODEGEN_CONTEXT_H

#include "IFJ_2022.h"

typedef char_ptr* char_ptr_ptr;
define_error(char_ptr_ptr);
DEFINE_VEC_PROTOTYPES(char_ptr, context);
DEFINE_VEC_ORD_PROTOTYPES(char_ptr, context);

error(vec_context) clone_context_vec(vec_context* src);

#endif //IFJ_2022_CODEGEN_CONTEXT_H
