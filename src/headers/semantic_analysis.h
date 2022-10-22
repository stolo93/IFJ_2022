/**
 * @file semantic_analysis.h
 * @author Vojtěch Novotný (xnovot1t@stud.fit.vutbr.cz)
 *
 * @brief Implementation of semantic analysis for IFJ22 language
 *
 * @date 2022-10-22
 *
 */

#include "stdbool.h"
#include "./error.h"
#include "./symtable.h"
#include "./syntax_tree.h"
#include "./IFJ_2022.h"

#define DEFAULT_SYMTABLE_SIZE 20

typedef htab_t_ptr* htab_t_ptr_ptr;
define_error(htab_t_ptr_ptr);
DEFINE_VEC_PROTOTYPES(htab_t_ptr, htab_t_ptr);
DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(htab_t_ptr, htab_t_ptr);

/**
 * @brief Function makes semantic checks on provided progtree
 * @param root pointer to progTree structure containing program statements
 *
 */
error( _Bool ) sendProgTree(PT_Node_ptr root);

/**
 * @brief Function goes through syntax_tree and saves all function definitions into symboltable
 * @param root starting node
 * @param functionDefinitionTable symbol table for function definitions
 *
 */
error(_Bool) getAllFunctionDefinitions(PT_Node_ptr root, htab_t_ptr table);

/**
 * @brief Function iterates through progTree nodes and checks nodes for semantic errors
 * @param node starting node
 *
 */
error( _Bool ) goThroughProgTreeNodes(PT_Node_ptr node);

/**
 * @brief Function checks for semantic errors on provided node
 * @param node provided node
 * @param functionContext context provided for when called in function definition
 *
 */
error( _Bool ) checkProgTreeNode(PT_Node_ptr node, htab_pair_t_ptr functionContext);

/**
 * @brief Function checks for semantic errors in function definition
 * @param node provided node
 * @param functionRecord symbol table record of function beeing checked
 *
 */
error(_Bool) checkFunctionDefinition(PT_Node_ptr node, htab_pair_t_ptr functionRecord);

/**
 * @brief Function checks for semantic errors in function call
 * @param node provided node
 *
 */
error( _Bool ) checkFunctionCall(PT_Node_ptr node);

/**
 * @brief Function checks for semantic errors in variable statement
 * @param node provided node
 *
 */
error( _Bool ) checkVariable(PT_Node_ptr node);

/**
 * @brief Function checks for semantic errors in if statement
 * @param node provided node
 * @param functionContext context provided for when called in function definition
 *
 */
error(_Bool) checkIfBlock(PT_Node_ptr node, htab_pair_t_ptr functionContext);

/**
 * @brief Function checks for semantic errors in while statement
 * @param node provided node
 * @param functionContext context provided for when called in function definition
 *
 */
error(_Bool) checkWhileBlock(PT_Node_ptr node, htab_pair_t_ptr functionContext);

/**
 * @brief Function checks for semantic errors in expression
 * @param node provided node
 *
 */
error( _Bool ) checkExpression(PT_Node_ptr node);

/**
 * @brief Function checks for semantic errors in return statement of a function
 * @param node provided node
 * @param functionRecord  pointer function record in symtable
 *
 */
error(_Bool) checkReturnSatement(PT_Node_ptr node, htab_pair_t_ptr functionRecord);

/**
 * @brief Function checks for semantic errors in body of if/else/while statements
 * @param node provided node
 * @param context provided for when called in function definition
 *
 */
error(_Bool) checkBodyNonTerminal(PT_Node_ptr node, htab_pair_t_ptr functionContext);

/**
 * @brief Function gets symTable containing all function definitions
 *
 */
error(htab_t_ptr) getMainSymTable();

/**
 * @brief Function goes through all symtables stored in symtable_vector and searches for item with key equal to parameter key
 * @param key identifier of entry
 *
 */
error(htab_pair_t_ptr) checkAllSymtableLevels(htab_key_t* key, sType type);

/**
 * @brief Function converts data types from tokenType to dType
 *
 * @param type input token type
 * @return enum dType converted input type
 */
dType tokenTypeToDataType(tokenType type);

/**
 * @brief Function finds furthest rightSibling PROG node in syntax_tree row
 * @param node starting node
 *
 */
error(PT_Node_ptr) findProgNode(PT_Node_t * node);

/**
 * @brief Function finds furthest rightSibling node in syntax_tree row
 * @param node starting node
 *
 */
error(PT_Node_ptr) findLastNodeOnRow(PT_Node_t * node);
