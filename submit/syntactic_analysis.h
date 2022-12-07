/****************************************************************
 * @name syntactic_analysis.h
 * @author Samuel Stolarik <xstola03@stud.fit.vutbr.cz>
 * @brief Header file for syntax analyser
 * @date 2022-11-02
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#include "./IFJ_2022.h"
#include "./syntax_tree.h"

//Grammar rules

/**
 * @brief Grammar rules <PROLOG> ->
 *
 */
error( _Bool ) SA_Prolog ( PT_Node_t* token_node );

/**
 * @brief Grammar rules <PROG> ->
 *
 */
error( _Bool ) SA_Prog ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <BODY> ->
 *
 */
error( _Bool ) SA_Body ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <STATEMENT> ->
 *
 */
error( _Bool ) SA_Statement ( PT_Node_t** token_node);

/**
 * @brief Grammar rules <RVAL> ->
 *
 */
error( _Bool ) SA_RVAL (PT_Node_t ** token_node);

/**
 * @brief Grammar rules <RET_VAL> ->
 *
 */
error( _Bool ) SA_RetVal ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <ARGS> ->
 *
 */
error( _Bool ) SA_Args ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <ARGS_NEXT> ->
 *
 */
error( _Bool ) SA_ArgsNext ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <ARG_TYPE> ->
 *
 */
error( _Bool ) SA_ARG_Type ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <PARAMS> ->
 *
 */
error( _Bool ) SA_Params ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <PARAMS_NEXT> ->
 *
 */
error( _Bool ) SA_ParamsNext ( PT_Node_t** token_node );

/**
 * @brief Grammar rules <TYPE> ->
 *
 */
error( _Bool ) SA_Type ( PT_Node_t* token_node );

/**
 * @brief Grammar rules <TERM> ->
 *
 */
error( _Bool ) SA_Term ( PT_Node_t* token_node );

/**
 * @brief @p condition specifies whether the expression analyzed, is used as a condition in if statement or while loop
 *
 */
error( _Bool ) SA_Expr ( PT_Node_t** token_node, bool cond_expr );


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
 * @brief Check precedence relation between the two tokens given as parameters.
 * Tokens given have to operators, otherwise the behaviour is undefined
 *
 * @param first
 * @param second
 * @return True if @p first has higher priority than @p second
 */
bool isHigherPrior ( tokenType first, tokenType second );

/**
 * @brief Read and destroy all tokens from set of tokens @p which
 * Used to skip expressions
 * @param which
 */
error(none) skipTokens ( const tokenType * which, PT_Node_t* token_node );

#endif
