/**
 * @file prog_tree.h
 * @author Samuel Stolarik (xstola03@stud.fit.vutbr.cz)
 *
 * @brief //TODO
 *
 * @date 2022-10-21
 *
 */

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
 * @brief Create new node with data @p data
 *
 * @return Pointer to an initialized node
 * @return NULL in case of malloc error
 */
error( PT_Node_ptr ) PT_CreateNode ( PT_Data_t data );


/**
 * @brief Insert a right-most sibling of node @p node ,
 * @p node should not be the root of a tree
 *
 */
error( PT_Node_ptr ) PT_AddSibling ( PT_Node_ptr node, PT_Data_t data );


/**
 * @brief Insert right-most child of a node @p node
 *
 */
error( PT_Node_ptr ) PT_AddChild ( PT_Node_ptr node, PT_Data_t data );

/**
 * @brief Create AST representing an expression in postfix notation
 *
 */
error( PT_Node_ptr ) PT_FromPostFix( vec_token_ptr * token_stack );

/**
 * @brief Print all terminals which are under @p node
 *
 * @param node
 */
void PT_PrintTerminals ( PT_Node_t * node );

/**
 * @brief Print token discriminant
 *
 * @param discriminant 
 */
void PT_PrintTokenType ( tokenType discriminant );

/**
 * @brief Deletes given node and everything underneath it
 *
 */
error( none ) PT_DeleteNode ( PT_Node_t  ** node );

#endif
