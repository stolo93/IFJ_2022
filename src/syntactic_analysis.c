/**
 * @file syntactic_analysis.c
 * @author Samuel Stolarik
 * @brief Implementation of syntax analyser with LL1 grammar
 *
 * @date 2022-11-02
 *
 */

#include "./headers/IFJ_2022.h"
#include "./headers/syntactic_analysis.h"
#include "./headers/scaner.h"

#include <stdbool.h>

error( _Bool ) SA_Prolog ( token_t * Token)
{
    if ( Token == NULL ){ return_error( INVALID_VAL, _Bool ); }

    bool Correct = false;

    //Simulate rule <PROLOG> -> prolog <PROG>
    if ( Token->discriminant == prolog )
    {
        error(token_ptr) tmp_token = getToken();
	get_value(token_ptr, cur_token, tmp_token, _Bool);

	//TODO insert cur_token into prog tree

	error(_Bool) tmp_result = SA_Prog(cur_token);
	free(cur_token); //TODO delete this, when inserted into progTree
	get_value(bool, res_sa_prog, tmp_result, _Bool);

		Correct = res_sa_prog;
	}

    return_value(Correct, _Bool);
}

error( _Bool ) SA_Prog ( token_t * Token )
{

}

error( _Bool ) SA_Body ( token_t * Token )
{

}

error( _Bool ) SA_Statement ( token_t * Token)
{

}

error( _Bool ) SA_RetVal ( token_t * Token )
{

}

error( _Bool ) SA_Args ( token_t * Token )
{

}

error( _Bool ) SA_ArgsNext ( token_t * Token )
{

}

error( _Bool ) SA_ARG_Type ( token_t * Token )
{

}

error( _Bool ) SA_Params ( token_t * Token )
{

}

error( _Bool ) SA_ParamsNext ( token_t * Token )
{

}

error( _Bool ) SA_Type ( token_t * Token )
{

}

error( _Bool ) SA_Term ( token_t * Token )
{

}

error( _Bool ) SA_Expr ( token_t * Token )
{

}


bool isInTokens ( tokenType Token, const tokenType * TokenList)
{
    if ( TokenList == NULL )
    {
        return false;
    }

    int i = 0;
    while ( TokenList[i] != N_VLD )
    {
        if ( Token == TokenList[i] )
        {
            return true;
        }
    }

    return false;
}

error(none) skipExpr ( const tokenType * which )
{
    if ( which == NULL ) {return_error(INVALID_VAL, none);}

    error(token_ptr) cur_token;
    bool first = true;

    do
    {
        if ( ! first )
        {
            free(cur_token._value);
        }

        cur_token = getToken();
        if ( is_error(cur_token) )
        {
            forward_error(cur_token, none);
        }

        first = false;
    } while ( isInTokens(cur_token._value->discriminant, which));

    returnToken(cur_token._value);

    return_none();
}

error(_Bool) isNextToken( tokenType TokenName )
{
	error(token_ptr) tmp_token = getToken();
	get_value(token_ptr, cur_token, tmp_token, _Bool);


	if ( cur_token->discriminant == TokenName )
	{
		//TODO insert cur_token into prog tree
		free(cur_token);
		return_value(true, _Bool);
	}

	else
	{
		returnToken(cur_token);
		return_value(false, _Bool);
	}
}
