/**
 * @file syntactic_analysis.h
 * @author Samuel Stolarik (xstola03@stud.fit.vutbr.cz)
 * @brief Header file for syntax analyser
 *
 * @date 2022-11-02
 *
 */

#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#include "./IFJ_2022.h"
#include "./syntax_tree.h"

//Grammar rules

error( _Bool ) SA_Prolog ( PT_Node_t* token_node );

error( _Bool ) SA_Prog ( PT_Node_t** token_node );

error( _Bool ) SA_Body ( PT_Node_t** token_node );

error( _Bool ) SA_Statement ( PT_Node_t** token_node);

error( _Bool ) SA_RetVal ( PT_Node_t** token_node );

error( _Bool ) SA_Args ( PT_Node_t** token_node );

error( _Bool ) SA_ArgsNext ( PT_Node_t** token_node );

error( _Bool ) SA_ARG_Type ( PT_Node_t** token_node );

error( _Bool ) SA_Params ( PT_Node_t** token_node );

error( _Bool ) SA_ParamsNext ( PT_Node_t** token_node );

error( _Bool ) SA_Type ( PT_Node_t* token_node );

error( _Bool ) SA_Term ( PT_Node_t* token_node );

/**
 * @todo For now SA_Expr only skips all tokens until it finds end of the expression,
 * which is either '{' (if the expression is used as a condition for either loop or if)
 * or ';' in case the expression is used as R-value in assignment
 *
 */
error( _Bool ) SA_Expr ( PT_Node_t* token_node );


//Utilities

/**
 * @brief Search for @p Token in @p TokenList
 *
 * @param Token, Token to be searched for
 * @param TokenList, List of availible tokens. The list HAS to end with N_VLD!
 * @return true @p Token was found, false otherwise
 */
bool isInTokens ( tokenType Token, const tokenType * TokenList );

/**
 * @brief Get next token and check if it's descriptot is equal to @p TokenName
 * If equal return true and insert token into the Abstract syntactic tree and return the new node through @p new_node
 * If not equal return false and return token back to scanner
 */
error(_Bool) isNextToken( tokenType TokenName, PT_Node_t* token_node );

/**
 * @brief Read and destroy all tokens from set of tokens @p which
 * Used to skip expressions
 * @param which
 */
error(none) skipTokens ( const tokenType * which, PT_Node_t* token_node );

#endif
