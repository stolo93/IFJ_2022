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
    if ( Token == NULL ) { return_error( INVALID_VAL, _Bool ); }

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
    if ( Token == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	bool Correct = false;
	error(_Bool) tmp_result;
	error(token_ptr) tmp_token;

    switch ( Token->discriminant )
    {
        // Function definition
        case functionT:
			// IdentOfFunc
			tmp_result = isNextToken(identOfFunct);
			get_value(bool, is_id_func_iof, tmp_result, _Bool);
			test_result(is_id_func_iof);

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_iof, tmp_result, _Bool);
			test_result(is_open_paren_iof);

			// <SA_PARAMS>
			tmp_token = getToken();
			get_value(token_ptr, params_token_iof, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Params(params_token_iof);
			free(params_token_iof);
			get_value(bool, res_sa_param_iof, tmp_result, _Bool);
			test_result(res_sa_param_iof);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_iof, tmp_result, _Bool);
			test_result(is_close_paren_iof);

			// Colon
			tmp_result = isNextToken(colon);
			get_value(bool, is_colon_iof, tmp_result, _Bool);
			test_result(is_colon_iof);

			// <TYPE>
			tmp_token = getToken();
			get_value(token_ptr, type_token_iof, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Type(type_token_iof);
			free(type_token_iof);
			get_value(bool, res_sa_type_iof, tmp_result, _Bool);
			test_result(res_sa_type_iof);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren_iof, tmp_result, _Bool);
			test_result(is_open_set_paren_iof);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token_iof, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token_iof);
			free(body_token_iof);
			get_value(bool, res_sa_body_iof, tmp_result, _Bool);
			test_result(res_sa_body_iof);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren_iof, tmp_result, _Bool);
			test_result(is_close_set_paren_iof);

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_iof, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Prog(prog_token_iof);
			free(prog_token_iof);
			get_value(bool, res_sa_prog_iof, tmp_result, _Bool);
			test_result(res_sa_prog_iof);

			// If everything got this far it's correct
			Correct = true;
            break;

        // If else statement
        case ifT:

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_if, tmp_result, _Bool);
			test_result(is_open_paren_if);

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_if, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_ARG_Type(arg_type_token_if);
			free(arg_type_token_if);
			get_value(bool, res_sa_arg_type_if, tmp_result, _Bool);
			test_result(res_sa_arg_type_if);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_if, tmp_result, _Bool);
			test_result(is_close_paren_if);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren1_if, tmp_result, _Bool);
			test_result(is_open_set_paren1_if);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token1_if, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token1_if);
			free(body_token1_if);
			get_value(bool, res_sa_body1_if, tmp_result, _Bool);
			test_result(res_sa_body1_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren1_if, tmp_result, _Bool);
			test_result(is_close_set_paren1_if);

			// else
			tmp_result = isNextToken(elseT);
			get_value(bool, is_else_if, tmp_result, _Bool);
			test_result(is_else_if);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren2_if, tmp_result, _Bool);
			test_result(is_open_set_paren2_if);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token2_if, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token2_if);
			free(body_token2_if);
			get_value(bool, res_sa_body2_if, tmp_result, _Bool);
			test_result(res_sa_body2_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren2_if, tmp_result, _Bool);
			test_result(is_close_set_paren2_if);

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_if, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Prog(prog_token_if);
			free(prog_token_if);
			get_value(bool, res_sa_prog_if, tmp_result, _Bool);
			test_result(res_sa_prog_if);

			// End of rule
			Correct = true;
            break;

        // While
        case whileT:

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_while, tmp_result, _Bool);
			test_result(is_open_paren_while);

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_while, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_ARG_Type(arg_type_token_while);
			free(arg_type_token_while);
			get_value(bool, res_sa_arg_type_while, tmp_result, _Bool);
			test_result(res_sa_arg_type_while);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_while, tmp_result, _Bool);
			test_result(is_close_paren_while);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren_while, tmp_result, _Bool);
			test_result(is_open_set_paren_while);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token_while, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token_while);
			free(body_token_while);
			get_value(bool, res_sa_body_while, tmp_result, _Bool);
			test_result(res_sa_body_while);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren_while, tmp_result, _Bool);
			test_result(is_close_set_paren_while);

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_while, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Prog(prog_token_while);
			free(prog_token_while);
			get_value(bool, res_sa_prog_while, tmp_result, _Bool);
			test_result(res_sa_prog_while);

			//End of rule while...
			Correct = true;
            break;

        // Function call
        case identOfFunct:

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_fcal, tmp_result, _Bool);
			test_result(is_open_paren_fcal);

			// <ARGS>
			tmp_token = getToken();
			get_value(token_ptr, args_token_fcal, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_Args(args_token_fcal);
			free(args_token_fcal);
			get_value(bool, res_sa_args_fcal, tmp_result, _Bool);
			test_result(res_sa_args_fcal);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_fcal, tmp_result, _Bool);
			test_result(is_close_paren_fcal);

			// Semicolon
			tmp_result = isNextToken(semicolon);
			get_value(bool, is_semicolon_fcal, tmp_result, _Bool);
			test_result(is_semicolon_fcal);

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_fcal, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Prog(prog_token_fcal);
			free(prog_token_fcal);
			get_value(bool, res_sa_prog_fcal, tmp_result, _Bool);
			test_result(res_sa_prog_fcal);

			// End of rule function call
			Correct = true;
            break;

        // Assignement
        case identOfVar:

			//eqOper
			tmp_result = isNextToken(EqOper);
			get_value(bool, is_eq_assign, tmp_result, _Bool);
			test_result(is_eq_assign);

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_assign, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_Expr(expr_token_assign);
			free(expr_token_assign);
			get_value(bool, res_sa_expr, tmp_result, _Bool);
			test_result(res_sa_expr);

			// Semicolon
			tmp_result = isNextToken(semicolon);
			get_value(bool, is_semicolon_assign, tmp_result, _Bool);
			test_result(is_semicolon_assign);

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_assign, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Prog(prog_token_assign);
			free(prog_token_assign);
			get_value(bool, res_sa_prog_assign, tmp_result, _Bool);
			test_result(res_sa_prog_assign);

			// End of assignment rule
			Correct = true;
            break;

        //Return statement
        case returnT:

			// <RET_VAL>
			tmp_token = getToken();
			get_value(token_ptr, ret_val_token_rv, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_RetVal(ret_val_token_rv);
			free(ret_val_token_rv);
			get_value(bool, res_sa_retval_rv, tmp_result, _Bool);
			test_result(res_sa_retval_rv);

			// Semicolon
			tmp_result = isNextToken(semicolon);
			get_value(bool, is_semicolon_rv, tmp_result, _Bool);
			test_result(is_semicolon_rv);

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_rv, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Prog(prog_token_rv);
			free(prog_token_rv);
			get_value(bool, res_sa_prog_rv, tmp_result, _Bool);
			test_result(res_sa_prog_rv);

			// End of rule return
			Correct = true;
            break;

        case endOfFile:
			//End of rule EOF
			Correct = true;
			break;

        //Expresion
        default:
            if ( isInTokens(Token->discriminant, expr_tokens) )
            {
                tmp_result = SA_Expr(Token);
				get_value(bool, res_sa_expr, tmp_result, _Bool);
				test_result(res_sa_expr);

				Correct = true;
            }
            break;
    }

    return_value(Correct, _Bool);
}

error( _Bool ) SA_Body ( token_t * Token )
{
    if ( Token == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	bool Correct = false;

    const tokenType tokenList_statement[]   = { identOfFunct, ifT, whileT, identOfVar, returnT, N_VLD};
    const tokenType tokenList_eps[]         = { closeSetParen , N_VLD};

    //Epsilon rule
    if ( isInTokens(Token->discriminant, tokenList_eps) )
    {
        Correct = true;
		returnToken(Token);
    }

    //Statement
    else if ( isInTokens(Token->discriminant, tokenList_statement) || isInTokens(Token->discriminant, expr_tokens))
    {
		error(_Bool) result = SA_Statement(Token);
		get_value(bool, res_sa_statement, result, _Bool);
		test_result(res_sa_statement);

		Correct = true;
    }

    return_value(Correct, _Bool);
}

error( _Bool ) SA_ST_List ( token_t * Token )
{

}

error( _Bool ) SA_Statement ( token_t * Token)
{
    if ( Token == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	bool Correct = false;
	error(_Bool) tmp_result;
	error(token_ptr) tmp_token;

    switch (Token->discriminant)
    {
        case identOfFunct:

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_fcal, tmp_result, _Bool);
			test_result(is_open_paren_fcal);

			// <ARGS>
			tmp_token = getToken();
			get_value(token_ptr, args_token_fcal, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_Args(args_token_fcal);
			free(args_token_fcal);
			get_value(bool, res_sa_args_fcal, tmp_result, _Bool);
			test_result(res_sa_args_fcal);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_fcal, tmp_result, _Bool);
			test_result(is_close_paren_fcal);

			// Semicolon
			tmp_result = isNextToken(semicolon);
			get_value(bool, is_semicolon_fcal, tmp_result, _Bool);
			test_result(is_semicolon_fcal);

			// End of rule function call
			Correct = true;
            break;

        // If Statement
        case ifT:

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_if, tmp_result, _Bool);
			test_result(is_open_paren_if);

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_if, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_ARG_Type(arg_type_token_if);
			free(arg_type_token_if);
			get_value(bool, res_sa_arg_type_if, tmp_result, _Bool);
			test_result(res_sa_arg_type_if);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_if, tmp_result, _Bool);
			test_result(is_close_paren_if);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren1_if, tmp_result, _Bool);
			test_result(is_open_set_paren1_if);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token1_if, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token1_if);
			free(body_token1_if);
			get_value(bool, res_sa_body1_if, tmp_result, _Bool);
			test_result(res_sa_body1_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren1_if, tmp_result, _Bool);
			test_result(is_close_set_paren1_if);

			// else
			tmp_result = isNextToken(elseT);
			get_value(bool, is_else_if, tmp_result, _Bool);
			test_result(is_else_if);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren2_if, tmp_result, _Bool);
			test_result(is_open_set_paren2_if);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token2_if, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token2_if);
			free(body_token2_if);
			get_value(bool, res_sa_body2_if, tmp_result, _Bool);
			test_result(res_sa_body2_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren2_if, tmp_result, _Bool);
			test_result(is_close_set_paren2_if);

			// End of rule
			Correct = true;
            break;

        // While
        case whileT:

			// Open Paren
			tmp_result = isNextToken(openParen);
			get_value(bool, is_open_paren_while, tmp_result, _Bool);
			test_result(is_open_paren_while);

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_while, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_ARG_Type(arg_type_token_while);
			free(arg_type_token_while);
			get_value(bool, res_sa_arg_type_while, tmp_result, _Bool);
			test_result(res_sa_arg_type_while);

			// Close paren
			tmp_result = isNextToken(closeParen);
			get_value(bool, is_close_paren_while, tmp_result, _Bool);
			test_result(is_close_paren_while);

			// Open set paren
			tmp_result = isNextToken(openSetParen);
			get_value(bool, is_open_set_paren_while, tmp_result, _Bool);
			test_result(is_open_set_paren_while);

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token_while, tmp_token, _Bool);
			//TODO insert token into progtree and delete the next free()

			tmp_result = SA_Body(body_token_while);
			free(body_token_while);
			get_value(bool, res_sa_body_while, tmp_result, _Bool);
			test_result(res_sa_body_while);

			// Close set paren
			tmp_result = isNextToken(closeSetParen);
			get_value(bool, is_close_set_paren_while, tmp_result, _Bool);
			test_result(is_close_set_paren_while);

			//End of rule while...
			Correct = true;
            break;

        // Assignment
        case identOfVar:

			//eqOper
			tmp_result = isNextToken(EqOper);
			get_value(bool, is_eq_assign, tmp_result, _Bool);
			test_result(is_eq_assign);

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_assign, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_Expr(expr_token_assign);
			free(expr_token_assign);
			get_value(bool, res_sa_expr, tmp_result, _Bool);
			test_result(res_sa_expr);

			// Semicolon
			tmp_result = isNextToken(semicolon);
			get_value(bool, is_semicolon_assign, tmp_result, _Bool);
			test_result(is_semicolon_assign);

			// End of assignment rule
			Correct = true;
            break;

        // Return statement
        case returnT:

			// <RET_VAL>
			tmp_token = getToken();
			get_value(token_ptr, ret_val_token_rv, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_RetVal(ret_val_token_rv);
			free(ret_val_token_rv);
			get_value(bool, res_sa_retval_rv, tmp_result, _Bool);
			test_result(res_sa_retval_rv);

			// Semicolon
			tmp_result = isNextToken(semicolon);
			get_value(bool, is_semicolon_rv, tmp_result, _Bool);
			test_result(is_semicolon_rv);

			// End of rule return
			Correct = true;
            break;

        //Expresion
        default:
            if ( isInTokens(Token->discriminant, expr_tokens) )
            {
                tmp_result = SA_Expr(Token);
				get_value(bool, res_sa_expr, tmp_result, _Bool);
				test_result(res_sa_expr);

				Correct = true;
            }
            break;
    }

    return_value(Correct, _Bool);

}

error( _Bool ) SA_RetVal ( token_t * Token )
{
    if ( Token == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	bool Correct = false;
    tokenType tokenList_eps[] = { semicolon, N_VLD };

    // Epsilon rule
    if ( isInTokens(Token->discriminant, tokenList_eps) )
    {
		Correct = true;
		returnToken(Token);
    }

    // Expression
    else if ( isInTokens(Token->discriminant, expr_tokens) )
    {
		error(_Bool) result = SA_Expr(Token);
		get_value(bool, res_sa_expr, result, _Bool);
		test_result(res_sa_expr);

		Correct = true;
    }

    return_value(Correct, _Bool);
}

error( _Bool ) SA_Args ( token_t * Token )
{
    if ( Token == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	bool Correct = false;
    const tokenType tokenList_arg_type[] = { identOfVar, nullT, decNum, integer, string, N_VLD };

    // Epsilon
    if ( Token->discriminant == closeParen )
    {
        Correct = true;
    }

    // <ARG_Type> <ARGS_NEXT>
    else if ( isInTokens(Token->discriminant, tokenList_arg_type) )
    {
		error(_Bool) tmp_result;

		//<ARGS_TYPE>
		tmp_result = SA_ARG_Type(Token);
		get_value(bool, res_sa_arg_type, tmp_result, _Bool);
		test_result(res_sa_arg_type);

		//<ARGS_NEXT>
		error(token_ptr) tmp_token = getToken();
		get_value(token_ptr, cur_token, tmp_token, _Bool);
		//TODO insert cur_token into prog tree and delete the next free()

		tmp_result = SA_ARG_Type(cur_token);
		free(cur_token);
		get_value(bool, res_sa_args_next, tmp_result, _Bool);
		test_result(res_sa_args_next);

		Correct = true;
    }

    return_value(Correct, _Bool);
}

error( _Bool ) SA_ArgsNext ( token_t * Token )
{
    if ( Token == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	bool Correct = false;

    switch (Token->discriminant)
    {

        // , <ARG_TYPE> <ARGS_NEXT>
        case comma:
            error(_Bool) tmp_result;
			error(token_ptr) tmp_token;

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, token_arg_type, tmp_token, _Bool);
			//TODO insert token into prog tree and delete the next free()

			tmp_result = SA_ARG_Type(token_arg_type);
			free(token_arg_type);
			get_value(bool, res_sa_arg_type, tmp_result, _Bool);
			test_result(res_sa_arg_type);

			Correct = true;
            break;

		//EPSILON
        case closeParen:
			Correct = true;
			returnToken(Token);
            break;
    }

    return_value(Correct, _Bool);
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
