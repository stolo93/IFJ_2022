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

//Grammar rules

error( _Bool ) SA_Prolog ( token_t * Token);

error( _Bool ) SA_Prog ( token_t * Token );

error( _Bool ) SA_Body ( token_t * Token );

error( _Bool ) SA_ST_List ( token_t * Token );

error( _Bool ) SA_Statement ( token_t * Token);

error( _Bool ) SA_Fcal ( token_t * Token );

error( _Bool ) SA_Fdef ( token_t * Token );

error( _Bool ) SA_Rval ( token_t * Token );

error( _Bool ) SA_RetVal ( token_t * Token );

error( _Bool ) SA_Args ( token_t * Token );

error( _Bool ) SA_ArgsNext ( token_t * Token );

error( _Bool ) SA_ARG_Type ( token_t * Token );

error( _Bool ) SA_Params ( token_t * Token );

error( _Bool ) SA_ParamsNext ( token_t * Token );

error( _Bool ) SA_Type ( token_t * Token );

error( _Bool ) SA_Term ( token_t * Token );

/**
 * @todo For now SA_Expr only skips all tokens until it finds end of the expression,
 * which is either '{' (if the expression is used as a condition for either loop or if)
 * or ';' in case the expression is used as R-value in assignment
 *
 */
error( _Bool ) SA_Expr ( token_t * Token );


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
 * If equal return true. //TODO insert to progtree
 * If not equal return false and return token back to scanner
 */
error(_Bool) isNextToken( tokenType TokenName );

/**
 * @brief Read and destroy all tokens from set of tokens @p which
 * Used to skip expressions
 * @param which
 */
error(none) skipTokens ( const tokenType * which );

#endif
