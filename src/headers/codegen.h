#ifndef IFJ_2022_CODEGEN_H
#define IFJ_2022_CODEGEN_H

#include "error.h"
#include "IFJ_2022.h"
#include "syntax_tree.h"
#include "interner.h"

#define tree_t PT_Node_t*

error(none) generate_code_from_syntax_tree(tree_t* tree);

#endif //IFJ_2022_CODEGEN_H
