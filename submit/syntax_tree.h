/****************************************************************
 * @name syntax_tree.h
 * @author Samuel Stolárik <xstola03@stud.fit.vutbr.cz>
 * @brief File contaning definitions of functions needed for AST implementation
 * @date 21.10.2022
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#ifndef _SYNTAX_TREE_H__
#define _SYNTAX_TREE_H__

#include "./IFJ_2022.h"
#include "./error.h"

#include <stdlib.h>

/**
 * @brief Stores either terminal or non-terminal symbol
 *
 */
typedef union symbol
{
    nonTerminal_t nonTerminal;
    token_t * terminal;
} symbol_t;


/**
 * @brief Data stored in every node of Program Tree
 *
 */
typedef struct PT_Data
{
    bool isTerminal; //true if this node contains terminal symbol, false otherwise
    symbol_t type;
} PT_Data_t;


/**
 * @brief Program Tree node
 *
 */
typedef struct PT_Node
{
    PT_Data_t data;
    struct PT_Node * leftChild;
    struct PT_Node * rightSibling;
} PT_Node_t;


typedef PT_Node_t* PT_Node_ptr;
typedef PT_Node_ptr* PT_Node_ptr_ptr;

define_error( PT_Node_ptr_ptr );
define_error( PT_Node_ptr );

DEFINE_VEC_PROTOTYPES(PT_Node_ptr, pnode);


/**
 * @brief Create node in program tree
 *
 */
error( PT_Node_ptr ) PT_CreateNode ( PT_Data_t data );


/**
 * @brief Create and insert the rightmost sibling of @p node containing @p data
 *
 */
error( PT_Node_ptr ) PT_AddSibling ( PT_Node_ptr node, PT_Data_t data );


/**
 * @brief Create and insert the rightmost child of @p node containing @p data
 *
 */
error( PT_Node_ptr ) PT_AddChild ( PT_Node_ptr node, PT_Data_t data );

/**
 * @brief Build program tree from  expression tokens in stored @p token_stack
 *
 */
error( PT_Node_ptr ) PT_FromPostFix( vec_token_ptr * token_stack );

/**
 * @brief Print all terminals which are either children or siblings of @p node
 *
 * @param node
 */
void PT_PrintTerminals ( PT_Node_t * node );

/**
 * @brief Print expression tree @p root in postfix notation
 *
 * @param root
 */
void PT_PrintExprTreeAsPostfix ( PT_Node_t * root );

/**
 * @brief Print type of token base on @p discriminant
 *
 * @param discriminant
 */
void PT_PrintTokenType ( tokenType discriminant );

/**
 * @brief Delete node and all children and sibling nodes of @p node
 *
 */
error( none ) PT_DeleteNode ( PT_Node_t  ** node );

#endif
