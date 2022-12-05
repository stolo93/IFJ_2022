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
#include <stdio.h>

#define test_result(result) do{ if( !result ) { return_error(ERROR_SYNTAX, _Bool); } } while(0)
#define test_error(er_obj, forw_type) do{ if( is_error(er_obj) ) { forward_error(er_obj, forw_type);} } while(0)
#define return_correct(Correct) do { if ( Correct ) {return_value(Correct, _Bool);} else {return_error(ERROR_SYNTAX, _Bool);} } while(0)

static const tokenType expr_tokens[] = {
								identOfVar, integer, decNum, string, nullT, plusSign, minusSign,
								multiply, division, concatenation, lessOper, lessOrEqOper,
								moreOper, moreOrEqOper, EqOper, notEqOper, openParen, closeParen,
								N_VLD
								};


error( _Bool ) SA_Prolog ( PT_Node_t * token_node )
{
    if ( token_node == NULL ) { return_error( INVALID_VAL, _Bool ); }

    bool Correct = false;

    //Simulate rule <PROLOG> -> prolog <PROG>
	if ( token_node->data.type.terminal->discriminant == prolog )
	{
		// Create <PROG> node
		PT_Data_t node_data = {.isTerminal = false, .type.nonTerminal = PROG};
		error(PT_Node_ptr) tmp_node =  PT_AddSibling(token_node, node_data);
		get_value(PT_Node_ptr, prog_node, tmp_node, _Bool);


		error(token_ptr) tmp_token = getToken();
		get_value(token_ptr, cur_token, tmp_token, _Bool);

		//Insert current token into the AST
		PT_Data_t new_node_data = {.isTerminal = true, .type.terminal = cur_token};
		tmp_node = PT_AddChild(prog_node, new_node_data);
		test_error(tmp_node, _Bool);

		error(_Bool) tmp_result = SA_Prog(&(prog_node->leftChild));
		get_value(bool, res_sa_prog, tmp_result, _Bool);

		Correct = res_sa_prog;
	}

    return_correct(Correct);
}

error( _Bool ) SA_Prog ( PT_Node_t ** token_node )
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
	error(_Bool) tmp_result;
	error(token_ptr) tmp_token;
	error(PT_Node_ptr) tmp_node;
	PT_Node_t* cur_node = *token_node;
	PT_Data_t tmp_node_data;

    switch ( (*token_node)->data.type.terminal->discriminant )
    {
        // Function definition
        case functionT:
		{
			// IdentOfFunc
			tmp_result = isNextToken(identOfFunct, cur_node);
			get_value(bool, is_id_func_iof, tmp_result, _Bool);
			test_result(is_id_func_iof);
			cur_node = cur_node->rightSibling;

			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_iof, tmp_result, _Bool);
			test_result(is_open_paren_iof);
			cur_node = cur_node->rightSibling;

			//Create <PARAMS> node in the ast
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PARAMS};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, params_node_fdef, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <SA_PARAMS>
			tmp_token = getToken();
			get_value(token_ptr, params_token_iof, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = params_token_iof};
			tmp_node = PT_AddChild(params_node_fdef, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Params(&(params_node_fdef->leftChild));
			get_value(bool, res_sa_param_iof, tmp_result, _Bool);
			test_result(res_sa_param_iof);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_iof, tmp_result, _Bool);
			test_result(is_close_paren_iof);
			cur_node = cur_node->rightSibling;

			// Colon
			tmp_result = isNextToken(colon, cur_node);
			get_value(bool, is_colon_iof, tmp_result, _Bool);
			test_result(is_colon_iof);
			cur_node = cur_node->rightSibling;


			//Create <TYPE> node in the ast
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = TYPE};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, type_node_fdef, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <TYPE>
			tmp_token = getToken();
			get_value(token_ptr, type_token_iof, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = type_token_iof};
			tmp_node = PT_AddChild(type_node_fdef, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Type(type_node_fdef->leftChild);
			get_value(bool, res_sa_type_iof, tmp_result, _Bool);
			test_result(res_sa_type_iof);

			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren_iof, tmp_result, _Bool);
			test_result(is_open_set_paren_iof);
			cur_node = cur_node->rightSibling;


			//Create node for <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_fdef, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token_iof, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token_iof};
			tmp_node = PT_AddChild(body_node_fdef, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_fdef->leftChild));
			get_value(bool, res_sa_body_iof, tmp_result, _Bool);
			test_result(res_sa_body_iof);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren_iof, tmp_result, _Bool);
			test_result(is_close_set_paren_iof);
			cur_node = cur_node->rightSibling;

			//Create node for <PROG>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, prog_node_fdef, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_iof, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_iof};
			tmp_node = PT_AddChild(prog_node_fdef, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Prog(&(prog_node_fdef->leftChild));
			get_value(bool, res_sa_prog_iof, tmp_result, _Bool);
			test_result(res_sa_prog_iof);

			// If everything got this far it's correct
			Correct = true;
            break;
		}
        // If else statement
        case ifT:
		{
			// Create node <EXPR>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = EXPR};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_if, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = expr_token_if};
			tmp_node = PT_AddChild(expr_node_if, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Expr(&(expr_node_if->leftChild), true);
			get_value(bool, res_sa_expr_if, tmp_result, _Bool);
			test_result(res_sa_expr_if);

			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren1_if, tmp_result, _Bool);
			test_result(is_open_set_paren1_if);
			cur_node = cur_node->rightSibling;

			// Create node for <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_if1, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token1_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token1_if};
			tmp_node = PT_AddChild(body_node_if1, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_if1->leftChild));
			get_value(bool, res_sa_body1_if, tmp_result, _Bool);
			test_result(res_sa_body1_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren1_if, tmp_result, _Bool);
			test_result(is_close_set_paren1_if);
			cur_node = cur_node->rightSibling;

			// else
			tmp_result = isNextToken(elseT, cur_node);
			get_value(bool, is_else_if, tmp_result, _Bool);
			test_result(is_else_if);
			cur_node = cur_node->rightSibling;

			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren2_if, tmp_result, _Bool);
			test_result(is_open_set_paren2_if);
			cur_node = cur_node->rightSibling;

			// Create node for the second <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_if2, tmp_node, _Bool);
			cur_node = cur_node ->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token2_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token2_if};
			tmp_node = PT_AddChild(body_node_if2, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_if2->leftChild));
			get_value(bool, res_sa_body2_if, tmp_result, _Bool);
			test_result(res_sa_body2_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren2_if, tmp_result, _Bool);
			test_result(is_close_set_paren2_if);
			cur_node = cur_node->rightSibling;

			// Create node for <PROG>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, prog_node_if, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_if};
			tmp_node = PT_AddChild(prog_node_if, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Prog(&(prog_node_if->leftChild));
			get_value(bool, res_sa_prog_if, tmp_result, _Bool);
			test_result(res_sa_prog_if);

			// End of rule
			Correct = true;
            break;
		}
        // While
        case whileT:
		{
			// Create node <EXPR>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = EXPR};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = expr_token_while};
			tmp_node = PT_AddChild(expr_node_while, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Expr(&(expr_node_while->leftChild), true);
			get_value(bool, res_sa_expr_while, tmp_result, _Bool);
			test_result(res_sa_expr_while);


			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren_while, tmp_result, _Bool);
			test_result(is_open_set_paren_while);
			cur_node = cur_node->rightSibling;

			// Create node for <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token_while};
			tmp_node = PT_AddChild(body_node_while, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_while->leftChild));
			get_value(bool, res_sa_body_while, tmp_result, _Bool);
			test_result(res_sa_body_while);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren_while, tmp_result, _Bool);
			test_result(is_close_set_paren_while);
			cur_node = cur_node->rightSibling;

			// Create node for <PROG>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, prog_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_while};
			tmp_node = PT_AddChild(prog_node_while, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Prog(&(prog_node_while->leftChild));
			get_value(bool, res_sa_prog_while, tmp_result, _Bool);
			test_result(res_sa_prog_while);

			//End of rule while...
			Correct = true;
            break;
		}
        // Function call
        case identOfFunct:
		{
			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_fcal, tmp_result, _Bool);
			test_result(is_open_paren_fcal);
			cur_node = cur_node->rightSibling;

			// Create node for <ARGS>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARGS};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, args_node_fcal, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARGS>
			tmp_token = getToken();
			get_value(token_ptr, args_token_fcal, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = args_token_fcal};
			tmp_node = PT_AddChild(args_node_fcal, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Args(&(args_node_fcal->leftChild));
			get_value(bool, res_sa_args_fcal, tmp_result, _Bool);
			test_result(res_sa_args_fcal);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_fcal, tmp_result, _Bool);
			test_result(is_close_paren_fcal);
			cur_node = cur_node->rightSibling;

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_fcal, tmp_result, _Bool);
			test_result(is_semicolon_fcal);
			cur_node = cur_node->rightSibling;

			// Create node for <PROG>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, prog_node_fcal, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_fcal, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_fcal};
			tmp_node = PT_AddChild(prog_node_fcal, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Prog(&(prog_node_fcal->leftChild));
			get_value(bool, res_sa_prog_fcal, tmp_result, _Bool);
			test_result(res_sa_prog_fcal);

			// End of rule function call
			Correct = true;
            break;
		}
        // Assignement
        case identOfVar:
		{
			//Assignment operator
			tmp_result = isNextToken(assigment, cur_node);
			get_value(bool, is_eq_assign, tmp_result, _Bool);

			// In case next token is not "=" this might be an expression starting with identOfVar
			if ( ! is_eq_assign )
			{
				cur_node->rightSibling = NULL;
				goto MIGHT_BE_AN_EXPRESSION;
			}
			// test_result(is_eq_assign);

			cur_node = cur_node->rightSibling;

			// Create node for <RVAL>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = RVAL};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, rval_node_assign, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <RVAL>
			tmp_token = getToken();
			get_value(token_ptr, rval_token_assign, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = rval_token_assign};
			tmp_node = PT_AddChild(rval_node_assign, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_RVAL(&(rval_node_assign->leftChild));
			get_value(bool, res_sa_expr, tmp_result, _Bool);
			test_result(res_sa_expr);

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_assign, tmp_result, _Bool);
			test_result(is_semicolon_assign);
			cur_node = cur_node->rightSibling;

			// Create node for <PROG>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, prog_node_assign, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_assign, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_assign};
			tmp_node = PT_AddChild(prog_node_assign, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Prog(&(prog_node_assign->leftChild));
			get_value(bool, res_sa_prog_assign, tmp_result, _Bool);
			test_result(res_sa_prog_assign);

			// End of assignment rule
			Correct = true;
            break;
		}
        //Return statement
        case returnT:
		{
			// Create node for <RET_VAL>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = RET_VAL};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, ret_val_node_ret, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <RET_VAL>
			tmp_token = getToken();
			get_value(token_ptr, ret_val_token_rv, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = ret_val_token_rv};
			tmp_node = PT_AddChild(ret_val_node_ret, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_RetVal(&(ret_val_node_ret->leftChild));
			get_value(bool, res_sa_retval_rv, tmp_result, _Bool);
			test_result(res_sa_retval_rv);

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_rv, tmp_result, _Bool);
			test_result(is_semicolon_rv);
			cur_node = cur_node->rightSibling;

			// Create node for <PROG>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, prog_node_ret, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <PROG>
			tmp_token = getToken();
			get_value(token_ptr, prog_token_rv, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_rv};
			tmp_node = PT_AddChild(prog_node_ret, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Prog(&(prog_node_ret->leftChild));
			get_value(bool, res_sa_prog_rv, tmp_result, _Bool);
			test_result(res_sa_prog_rv);

			// End of rule return
			Correct = true;
            break;
		}
        case endOfFile:
		{
			//End of rule EOF
			Correct = true;
			break;
		}
        //Expresion
        default:
		{
			MIGHT_BE_AN_EXPRESSION:
            if ( isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens) )
            {
				// Create <EXPR> node and insert it as a parent of the @p token_node
				PT_Data_t expr_node_data = {.isTerminal = false, .type.nonTerminal = EXPR};
				error(PT_Node_ptr) tmp_node = PT_CreateNode(expr_node_data);
				get_value(PT_Node_ptr, expr_node, tmp_node, _Bool);

				expr_node->leftChild = *token_node;
				*token_node = expr_node;

                tmp_result = SA_Expr(&(expr_node->leftChild), false);
				get_value(bool, res_sa_expr, tmp_result, _Bool);
				test_result(res_sa_expr);
				cur_node = expr_node;

				// Semicolon
				tmp_result = isNextToken(semicolon, cur_node);
				get_value(bool, is_semicolon_rv, tmp_result, _Bool);
				test_result(is_semicolon_rv);
				cur_node = cur_node->rightSibling;

				//Create node for <PROG>
				tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = PROG};
				tmp_node = PT_AddSibling(cur_node, tmp_node_data);
				get_value(PT_Node_ptr, prog_node_expr, tmp_node, _Bool);
				cur_node = cur_node->rightSibling;

				// <PROG>
				tmp_token = getToken();
				get_value(token_ptr, prog_token_expr, tmp_token, _Bool);
				tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = prog_token_expr};
				tmp_node = PT_AddChild(prog_node_expr, tmp_node_data);
				test_error(tmp_node, _Bool);

				tmp_result = SA_Prog(&(prog_node_expr->leftChild));
				get_value(bool, res_sa_prog_expr, tmp_result, _Bool);
				test_result(res_sa_prog_expr);

				// If everything got this far it's correct
				Correct = true;
            }
            break;
		}
    }

    return_correct(Correct);
}

error( _Bool ) SA_Body ( PT_Node_t ** token_node )
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;

    static const tokenType tokenList_statement[]   = { identOfFunct, ifT, whileT, identOfVar, returnT, N_VLD};
    static const tokenType tokenList_eps[]         = { closeSetParen , N_VLD};

    //Epsilon rule
    if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_eps) )
    {
        Correct = true;
		returnToken((*token_node)->data.type.terminal);
		free(*token_node);
		*token_node = NULL;
    }

    //Statement and next body
    else if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_statement) || isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens))
    {
		// Create <STATEMENT> node and insert it as a parent of current token node
		PT_Data_t tmp_node_data = {.isTerminal = false, .type.nonTerminal = STATEMENT};
		error(PT_Node_ptr) tmp_node = PT_CreateNode(tmp_node_data);
		get_value(PT_Node_ptr, stat_node, tmp_node, _Bool);

		stat_node->leftChild = *token_node;
		*token_node = stat_node;

		error(_Bool) result = SA_Statement(&(stat_node->leftChild));
		get_value(bool, res_sa_statement, result, _Bool);
		test_result(res_sa_statement);

		error(token_ptr) tmp_token;
		error(_Bool) tmp_result;

		// Create node for <BODY>
		tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
		tmp_node = PT_AddSibling(stat_node, tmp_node_data);
		get_value(PT_Node_ptr, body_node, tmp_node, _Bool);

		// <BODY>
		tmp_token = getToken();
		get_value(token_ptr, body_token_while, tmp_token, _Bool);
		tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token_while};
		tmp_node = PT_AddChild(body_node, tmp_node_data);
		test_error(tmp_node, _Bool);

		tmp_result = SA_Body(&(body_node->leftChild));
		get_value(bool, res_sa_body_while, tmp_result, _Bool);
		test_result(res_sa_body_while);

		Correct = true;
    }

    return_correct(Correct);
}

error( _Bool ) SA_Statement ( PT_Node_t ** token_node)
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }

	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
	error(_Bool) tmp_result;
	error(token_ptr) tmp_token;
	error(PT_Node_ptr) tmp_node;
	PT_Node_t* cur_node = *token_node;
	PT_Data_t tmp_node_data;

    switch ((*token_node)->data.type.terminal->discriminant)
    {
        case identOfFunct:
		{
			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_fcal, tmp_result, _Bool);
			test_result(is_open_paren_fcal);
			cur_node = cur_node->rightSibling;

			// Create node for <ARGS>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARGS};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, args_node_fcal, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARGS>
			tmp_token = getToken();
			get_value(token_ptr, args_token_fcal, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = args_token_fcal};
			tmp_node = PT_AddChild(args_node_fcal, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Args(&(args_node_fcal->leftChild));
			get_value(bool, res_sa_args_fcal, tmp_result, _Bool);
			test_result(res_sa_args_fcal);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_fcal, tmp_result, _Bool);
			test_result(is_close_paren_fcal);
			cur_node = cur_node->rightSibling;

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_fcal, tmp_result, _Bool);
			test_result(is_semicolon_fcal);
			cur_node = cur_node->rightSibling;

			// End of rule function call
			Correct = true;
            break;
		}
        // If Statement
        case ifT:
		{
			// Create node <EXPR>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = EXPR};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_if, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = expr_token_if};
			tmp_node = PT_AddChild(expr_node_if, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Expr(&(expr_node_if->leftChild), true);
			get_value(bool, res_sa_expr_if, tmp_result, _Bool);
			test_result(res_sa_expr_if);

			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren1_if, tmp_result, _Bool);
			test_result(is_open_set_paren1_if);
			cur_node = cur_node->rightSibling;

			// Create node for <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_if1, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token1_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token1_if};
			tmp_node = PT_AddChild(body_node_if1, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_if1->leftChild));
			get_value(bool, res_sa_body1_if, tmp_result, _Bool);
			test_result(res_sa_body1_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren1_if, tmp_result, _Bool);
			test_result(is_close_set_paren1_if);
			cur_node = cur_node->rightSibling;

			// else
			tmp_result = isNextToken(elseT, cur_node);
			get_value(bool, is_else_if, tmp_result, _Bool);
			test_result(is_else_if);
			cur_node = cur_node->rightSibling;

			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren2_if, tmp_result, _Bool);
			test_result(is_open_set_paren2_if);
			cur_node = cur_node->rightSibling;

			// Create node for the second <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_if2, tmp_node, _Bool);
			cur_node = cur_node ->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token2_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token2_if};
			tmp_node = PT_AddChild(body_node_if2, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_if2->leftChild));
			get_value(bool, res_sa_body2_if, tmp_result, _Bool);
			test_result(res_sa_body2_if);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren2_if, tmp_result, _Bool);
			test_result(is_close_set_paren2_if);
			cur_node = cur_node->rightSibling;

			// End of rule
			Correct = true;
            break;
		}
        // While
        case whileT:
		{
			// Create node <EXPR>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = EXPR};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = expr_token_while};
			tmp_node = PT_AddChild(expr_node_while, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Expr(&(expr_node_while->leftChild), true);
			get_value(bool, res_sa_expr_while, tmp_result, _Bool);
			test_result(res_sa_expr_while);

			// Open set paren
			tmp_result = isNextToken(openSetParen, cur_node);
			get_value(bool, is_open_set_paren_while, tmp_result, _Bool);
			test_result(is_open_set_paren_while);
			cur_node = cur_node->rightSibling;

			// Create node for <BODY>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = BODY};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, body_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <BODY>
			tmp_token = getToken();
			get_value(token_ptr, body_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = body_token_while};
			tmp_node = PT_AddChild(body_node_while, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_Body(&(body_node_while->leftChild));
			get_value(bool, res_sa_body_while, tmp_result, _Bool);
			test_result(res_sa_body_while);

			// Close set paren
			tmp_result = isNextToken(closeSetParen, cur_node);
			get_value(bool, is_close_set_paren_while, tmp_result, _Bool);
			test_result(is_close_set_paren_while);
			cur_node = cur_node->rightSibling;

			//End of rule while...
			Correct = true;
            break;
		}
        // Assignment
        case identOfVar:
		{
			//Assignment operator
			tmp_result = isNextToken(assigment, cur_node);
			get_value(bool, is_eq_assign, tmp_result, _Bool);

			// In case next token is not "=" this might be an expression starting with identOfVar
			if ( ! is_eq_assign )
			{
				cur_node->rightSibling = NULL;
				goto MIGHT_BE_AN_EXPRESSION;
			}

			cur_node = cur_node->rightSibling;

			// Create node for <RVAL>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = RVAL};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, rval_node_assign, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <RVAL>
			tmp_token = getToken();
			get_value(token_ptr, rval_token_assign, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = rval_token_assign};
			tmp_node = PT_AddChild(rval_node_assign, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_RVAL(&(rval_node_assign->leftChild));
			get_value(bool, res_sa_rval, tmp_result, _Bool);
			test_result(res_sa_rval);

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_assign, tmp_result, _Bool);
			test_result(is_semicolon_assign);
			cur_node = cur_node->rightSibling;

			// End of assignment rule
			Correct = true;
            break;
		}
        // Return statement
        case returnT:
		{
			// Create node for <RET_VAL>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = RET_VAL};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, ret_val_node_ret, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <RET_VAL>
			tmp_token = getToken();
			get_value(token_ptr, ret_val_token_rv, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = ret_val_token_rv};
			tmp_node = PT_AddChild(ret_val_node_ret, tmp_node_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_RetVal(&(ret_val_node_ret->leftChild));
			get_value(bool, res_sa_retval_rv, tmp_result, _Bool);
			test_result(res_sa_retval_rv);

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_rv, tmp_result, _Bool);
			test_result(is_semicolon_rv);
			cur_node = cur_node->rightSibling;

			// End of rule return
			Correct = true;
            break;
		}

        //Expresion
        default:
		{
		MIGHT_BE_AN_EXPRESSION:
            if ( isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens) )
            {
				// Create <EXPR> node and insert it as a parent of the @p token_node
				PT_Data_t expr_node_data = {.isTerminal = false, .type.nonTerminal = EXPR};
				error(PT_Node_ptr) tmp_node = PT_CreateNode(expr_node_data);
				get_value(PT_Node_ptr, expr_node, tmp_node, _Bool);

				expr_node->leftChild = *token_node;
				*token_node = expr_node;

                tmp_result = SA_Expr(&(expr_node->leftChild), false);
				get_value(bool, res_sa_expr, tmp_result, _Bool);
				test_result(res_sa_expr);
				cur_node = expr_node;

				// Semicolon
				tmp_result = isNextToken(semicolon, cur_node);
				get_value(bool, is_semicolon_rv, tmp_result, _Bool);
				test_result(is_semicolon_rv);
				cur_node = cur_node->rightSibling;

				Correct = true;
            }
            break;
		}
    }

    return_correct(Correct);

}

error( _Bool ) SA_RVAL ( PT_Node_t ** token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
	PT_Node_t * cur_node = *token_node;
	PT_Data_t tmp_node_data;

	error(_Bool) tmp_result;
	error(PT_Node_ptr) tmp_node;
	error(token_ptr) tmp_token;

	// Fucntion call
	if ( (*token_node)->data.type.terminal->discriminant == identOfFunct )
	{
		// Open Paren
		tmp_result = isNextToken(openParen, cur_node);
		get_value(bool, is_open_paren_fcal, tmp_result, _Bool);
		test_result(is_open_paren_fcal);
		cur_node = cur_node->rightSibling;

		// Create node for <ARGS>
		tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARGS};
		tmp_node = PT_AddSibling(cur_node, tmp_node_data);
		get_value(PT_Node_ptr, args_node_fcal, tmp_node, _Bool);
		cur_node = cur_node->rightSibling;

		// <ARGS>
		tmp_token = getToken();
		get_value(token_ptr, args_token_fcal, tmp_token, _Bool);
		tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = args_token_fcal};
		tmp_node = PT_AddChild(args_node_fcal, tmp_node_data);
		test_error(tmp_node, _Bool);

		tmp_result = SA_Args(&(args_node_fcal->leftChild));
		get_value(bool, res_sa_args_fcal, tmp_result, _Bool);
		test_result(res_sa_args_fcal);

		// Close paren
		tmp_result = isNextToken(closeParen, cur_node);
		get_value(bool, is_close_paren_fcal, tmp_result, _Bool);
		test_result(is_close_paren_fcal);
		cur_node = cur_node->rightSibling;

		// End of rule function call
		Correct = true;
	}

	// Expression
	else if ( isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens) )
	{
		// Create node for <EXPR> and insert it as a parent of @p token_node
		PT_Data_t expr_node_data = {.isTerminal = false, .type.nonTerminal = EXPR};
		error(PT_Node_ptr) tmp_node = PT_CreateNode(expr_node_data);
		get_value(PT_Node_ptr, expr_node, tmp_node, _Bool);

		expr_node->leftChild = *token_node;
		*token_node = expr_node;

		error(_Bool) result = SA_Expr(&(expr_node->leftChild), false);
		get_value(bool, res_sa_expr, result, _Bool);
		test_result(res_sa_expr);

		Correct = true;
	}

	return_correct(Correct);
}

error( _Bool ) SA_RetVal ( PT_Node_t ** token_node )
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
    tokenType tokenList_eps[] = { semicolon, N_VLD };

    // Epsilon rule
    if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_eps) )
    {
		Correct = true;
		returnToken((*token_node)->data.type.terminal);
		free(*token_node);
		*token_node = NULL;
    }

    // Expression
    else if ( isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens) )
    {
		// Create node for <EXPR> and insert it as a parent of @p token_node
		PT_Data_t expr_node_data = {.isTerminal = false, .type.nonTerminal = EXPR};
		error(PT_Node_ptr) tmp_node = PT_CreateNode(expr_node_data);
		get_value(PT_Node_ptr, expr_node, tmp_node, _Bool);

		expr_node->leftChild = *token_node;
		*token_node = expr_node;

		error(_Bool) result = SA_Expr(&(expr_node->leftChild), false);
		get_value(bool, res_sa_expr, result, _Bool);
		test_result(res_sa_expr);

		Correct = true;
    }

    return_correct(Correct);
}

error( _Bool ) SA_Args ( PT_Node_t ** token_node )
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
    static const tokenType tokenList_arg_type[] = { identOfVar, nullT, decNum, integer, string, N_VLD };

    // Epsilon
    if ( (*token_node)->data.type.terminal->discriminant == closeParen )
    {
		returnToken((*token_node)->data.type.terminal);
		free(*token_node);
		*token_node = NULL;
        Correct = true;
    }

    // <ARG_Type> <ARGS_NEXT>
    else if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_arg_type) )
    {
		error(_Bool) tmp_result;
		error(PT_Node_ptr) tmp_node;

		// Create node for <ARGS_TYPE> and insert it as a parent of @p token_node
		PT_Data_t arg_type_node_data = {.isTerminal = false, .type.nonTerminal = ARG_TYPE};
		tmp_node = PT_CreateNode(arg_type_node_data);
		get_value(PT_Node_ptr, arg_type_node, tmp_node, _Bool);

		arg_type_node->leftChild = *token_node;
		*token_node = arg_type_node;

		//<ARGS_TYPE>
		tmp_result = SA_ARG_Type(&arg_type_node->leftChild);
		get_value(bool, res_sa_arg_type, tmp_result, _Bool);
		test_result(res_sa_arg_type);


		// Create node for <ARGS_NEXT>
		PT_Data_t args_next_node_data = {.isTerminal = false, .type.nonTerminal = ARGS_NEXT};
		tmp_node = PT_AddSibling(arg_type_node, args_next_node_data);
		get_value(PT_Node_ptr, args_next_node, tmp_node, _Bool);

		//<ARGS_NEXT>
		error(token_ptr) tmp_token = getToken();
		get_value(token_ptr, cur_token, tmp_token, _Bool);
		PT_Data_t args_next_child_data = {.isTerminal = true, .type.terminal = cur_token};
		tmp_node = PT_AddChild(args_next_node, args_next_child_data);
		test_error(tmp_node, _Bool);

		tmp_result = SA_ArgsNext(&(args_next_node->leftChild));
		get_value(bool, res_sa_args_next, tmp_result, _Bool);
		test_result(res_sa_args_next);

		Correct = true;
    }

    return_correct(Correct);
}

error( _Bool ) SA_ArgsNext ( PT_Node_t ** token_node )
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;

    switch ((*token_node)->data.type.terminal->discriminant)
    {

        // , <ARG_TYPE> <ARGS_NEXT>
        case comma:
		{
            error(_Bool) tmp_result;
			error(token_ptr) tmp_token;
			error(PT_Node_ptr) tmp_node;
			PT_Node_t * cur_node = *token_node;

			// Create node for <ARG_TYPE>
			PT_Data_t arg_type_node_data = {.isTerminal = false, .type.nonTerminal = ARG_TYPE};
			tmp_node = PT_AddSibling(cur_node, arg_type_node_data);
			get_value(PT_Node_ptr, arg_type_node, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, token_arg_type, tmp_token, _Bool);
			PT_Data_t arg_type_child_data = {.isTerminal = true, .type.terminal = token_arg_type};
			tmp_node = PT_AddChild(arg_type_node, arg_type_child_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_ARG_Type(&(arg_type_node->leftChild));
			get_value(bool, res_sa_arg_type, tmp_result, _Bool);
			test_result(res_sa_arg_type);

			// Create node for <ARGS_NEXT>
			PT_Data_t args_next_node_data = {.isTerminal = false, .type.nonTerminal = ARGS_NEXT};
			tmp_node = PT_AddSibling(cur_node, args_next_node_data);
			get_value(PT_Node_ptr, args_next_node, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARGS_NEXT>
			tmp_token = getToken();
			get_value(token_ptr, token_args_next, tmp_token, _Bool);
			PT_Data_t args_next_child_data = {.isTerminal = true, .type.terminal = token_args_next};
			tmp_node = PT_AddChild(args_next_node, args_next_child_data);
			test_error(tmp_node, _Bool);

			tmp_result = SA_ArgsNext(&(args_next_node->leftChild));
			get_value(bool, res_sa_args_next, tmp_result, _Bool);
			test_result(res_sa_args_next);

			Correct = true;
            break;
		}
		//EPSILON
        case closeParen:
		{
			Correct = true;
			returnToken((*token_node)->data.type.terminal);
			free(*token_node);
			*token_node = NULL;
            break;
		}
		default:
			break;
    }

    return_correct(Correct);
}

error( _Bool ) SA_ARG_Type ( PT_Node_t ** token_node )
{
    if ( token_node == NULL )
    {
        return_error(INVALID_VAL, _Bool);
    }
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
    static const tokenType tokenList_term[] = {nullT, decNum, integer, string, N_VLD};

    // ID of variable
    if ( (*token_node)->data.type.terminal->discriminant == identOfVar )
    {
        Correct = true;
    }

    // Literals
    else if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_term) )
    {
		// Create node for <TERM> and insert it as a parent of @p token_node
		PT_Data_t term_node_data = {.isTerminal = false, .type.nonTerminal = TERM};
		error(PT_Node_ptr) tmp_node = PT_CreateNode(term_node_data);
		get_value(PT_Node_ptr, term_node, tmp_node, _Bool);

		term_node->leftChild = *token_node;
		*token_node = term_node;

        error(_Bool) tmp_result = SA_Term(term_node->leftChild);
		get_value(bool, res_sa_term, tmp_result, _Bool);
		test_result(res_sa_term);

		Correct = true;
    }

    return_correct(Correct);
}

error( _Bool ) SA_Params ( PT_Node_t ** token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
	const tokenType tokenList_type[] = { identOfType, identOfTypeN, N_VLD };

	// EPS
	if ( (*token_node)->data.type.terminal->discriminant == closeParen )
	{
		Correct = true;
		returnToken((*token_node)->data.type.terminal);
		free(*token_node);
		*token_node = NULL;
	}

	// <TYPE>...
	else if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_type) )
	{
		PT_Node_t * cur_node;
		error(_Bool) tmp_result;
		error(token_ptr) tmp_token;
		error(PT_Node_ptr) tmp_node;


		// Create node for <TYPE> and insert it as a parent of @p node_token
		PT_Data_t type_node_data = {.isTerminal = false, .type.nonTerminal = TYPE};
		tmp_node = PT_CreateNode(type_node_data);
		get_value(PT_Node_ptr, type_node, tmp_node, _Bool);

		type_node->leftChild = *token_node;
		*token_node = type_node;

		tmp_result = SA_Type(type_node->leftChild);
		get_value(bool, res_sa_type, tmp_result, _Bool);
		test_result(res_sa_type);

		// identOfVar
		cur_node = type_node;
		tmp_result = isNextToken(identOfVar, cur_node);
		get_value(bool, is_iovar, tmp_result, _Bool);
		test_result(is_iovar);
		cur_node = cur_node->rightSibling;

		// Create node for <PARAMS_NEXT>
		PT_Data_t params_next_node_data = {.isTerminal = false, .type.nonTerminal = PARAMS_NEXT};
		tmp_node = PT_AddSibling(cur_node, params_next_node_data);
		get_value(PT_Node_ptr, params_next_node, tmp_node, _Bool);
		cur_node = cur_node->rightSibling;

		// <PARAMS_NEXT>
		tmp_token = getToken();
		get_value(token_ptr, params_next_token, tmp_token, _Bool);
		PT_Data_t params_next_child_data = {.isTerminal = true, .type.terminal = params_next_token};
		tmp_node = PT_AddChild(params_next_node, params_next_child_data);
		test_error(tmp_node, _Bool);

		tmp_result = SA_ParamsNext(&(params_next_node->leftChild));
		get_value(bool, res_sa_params_next, tmp_result, _Bool);
		test_result(res_sa_params_next);

		Correct = true;
	}

	return_correct(Correct);

}

error( _Bool ) SA_ParamsNext ( PT_Node_t ** token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;

	// EPS
	if ( (*token_node)->data.type.terminal->discriminant == closeParen )
	{
		Correct = true;
		returnToken((*token_node)->data.type.terminal);
		free(*token_node);
		*token_node = NULL;
	}

	// , <TYPE>...
	else if ( (*token_node)->data.type.terminal->discriminant == comma )
	{
		error(_Bool) tmp_result;
		error(token_ptr) tmp_token;
		PT_Node_t * cur_node = *token_node;
		error(PT_Node_ptr) tmp_node;

		// Create node for <TYPE>
		PT_Data_t type_node_data = {.isTerminal = false, .type.nonTerminal = TYPE};
		tmp_node = PT_AddSibling(cur_node, type_node_data);
		get_value(PT_Node_ptr, type_node, tmp_node, _Bool);
		cur_node = cur_node->rightSibling;

		// <TYPE>
		tmp_token = getToken();
		get_value(token_ptr, type_token, tmp_token, _Bool);
		PT_Data_t type_child_data = {.isTerminal = true, .type.terminal = type_token};
		tmp_node = PT_AddChild(type_node, type_child_data);
		test_error(tmp_node, _Bool);

		tmp_result = SA_Type(type_node->leftChild);
		get_value(bool, res_sa_type, tmp_result, _Bool);
		test_result(res_sa_type);

		// identOfVar
		tmp_result = isNextToken(identOfVar, cur_node);
		get_value(bool, is_ident_of_var, tmp_result, _Bool);
		test_result(is_ident_of_var);
		cur_node = cur_node->rightSibling;

		//Create node for <PARAMS_NEXT>
		PT_Data_t params_next_data = {.isTerminal = false, .type.nonTerminal = PARAMS_NEXT};
		tmp_node = PT_AddSibling(cur_node, params_next_data);
		get_value(PT_Node_ptr, params_next_node, tmp_node, _Bool);
		cur_node = cur_node->rightSibling;

		// <PARAMS_NEXT>
		tmp_token = getToken();
		get_value(token_ptr, params_next_token, tmp_token, _Bool);
		PT_Data_t params_next_child_data = {.isTerminal = true, .type.terminal = params_next_token};
		tmp_node = PT_AddChild(params_next_node, params_next_child_data);
		test_error(tmp_node, _Bool);

		tmp_result = SA_ParamsNext(&(params_next_node->leftChild));
		get_value(bool, res_sa_params_next, tmp_result, _Bool);
		test_result(res_sa_params_next);

		Correct = true;
	}

	return_correct(Correct);
}

error( _Bool ) SA_Type ( PT_Node_t * token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;

	if ( token_node->data.type.terminal->discriminant == identOfType || token_node->data.type.terminal->discriminant == identOfTypeN )
	{
		Correct = true;
	}

	return_correct(Correct);
}

error( _Bool ) SA_Term ( PT_Node_t * token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;
	const tokenType tokenList_term[] = { nullT, decNum, integer, string, N_VLD };

	if ( isInTokens(token_node->data.type.terminal->discriminant, tokenList_term) )
	{
		Correct = true;
	}

	return_correct(Correct);
}

error( _Bool ) SA_Expr ( PT_Node_t** token_node, bool cond_expr )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}
	if ( *token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = true;

	vec_token_ptr postfix_expr = new_vec_token_ptr();
	vec_token_ptr aux_postfix = new_vec_token_ptr();

	// Token categories

	static const tokenType operators[] = { plusSign, minusSign, multiply, division, concatenation, lessOper, lessOrEqOper, moreOper, moreOrEqOper, EqOper, notEqOper , N_VLD};
	static const tokenType operands[] = { nullT, identOfVar, integer, decNum, string, N_VLD};
	static const tokenType end_exp[] = { openSetParen, semicolon, N_VLD };
	static const tokenType start_exp[] = { identOfVar, integer, decNum, string, nullT, openParen, N_VLD };
	static const tokenType expr_last[] = { identOfVar, integer, decNum, string, nullT, closeParen, N_VLD};

	// Acceptable tokens before a particular token

	static const tokenType bef_operand[] = { plusSign, minusSign, multiply, division, concatenation, lessOper, lessOrEqOper, moreOper, moreOrEqOper, EqOper, notEqOper, openParen, N_VLD };
	static const tokenType bef_operator[] = { nullT, identOfVar, integer, decNum, string, closeParen, N_VLD };
	static const tokenType bef_openParen[] = { plusSign, minusSign, multiply, division, concatenation, lessOper, lessOrEqOper, moreOper, moreOrEqOper, EqOper, notEqOper, openParen, N_VLD };
	static const tokenType bef_closeParen[] = { nullT, identOfVar, integer, decNum, string, closeParen, N_VLD};


	error(token_ptr) tmp_token;
	error(token_ptr_ptr) aux_token_tmp;
	token_t * cur_token = (*token_node)->data.type.terminal;
	token_t * prev_token;
	token_t * aux_token;
	size_t aux_vec_len;

	// First token
	// If analysed expression is used as if or while condition it has to start with (
	if ( !isInTokens(cur_token->discriminant, start_exp) || (cond_expr && cur_token->discriminant != openParen))
	{
		vec_token_ptr_destroy(&postfix_expr);
		vec_token_ptr_destroy(&aux_postfix);
		return_correct(false);
	}

	// Push the first token onto the correct stack and free it's node in the ast
	if ( cur_token->discriminant == openParen )
	{
		vec_token_ptr_push_back(&aux_postfix, cur_token);
	}
	else
	{
		vec_token_ptr_push_back(&postfix_expr, cur_token);
	}
	free(*token_node);
	*token_node = NULL;

	// Convert the rest of the expression

	prev_token = cur_token;
	tmp_token = getToken();
	if ( is_error(tmp_token) )
	{
		vec_token_ptr_destroy(&postfix_expr);
		vec_token_ptr_destroy(&aux_postfix);
		forward_error(tmp_token, _Bool);
	}
	cur_token = tmp_token._value;


	// Convert infix to postfix and check expression syntax

	while ( !isInTokens(cur_token->discriminant, end_exp) && isInTokens(cur_token->discriminant, expr_tokens) )
	{
		// Operand
		if (isInTokens(cur_token->discriminant, operands) )
		{
			if ( ! isInTokens(prev_token->discriminant, bef_operand) )
			{
				Correct = false;
				break;
			}
			vec_token_ptr_push_back(&postfix_expr, cur_token);

		}

		// Operator
		else if (isInTokens(cur_token->discriminant, operators) )
		{
			if ( ! isInTokens(prev_token->discriminant, bef_operator) )
			{
				Correct = false;
				break;
			}

			aux_vec_len = vec_token_ptr_len(&aux_postfix);
			bool inserted = false;

			while ( !inserted )
			{
				if ( aux_vec_len == 0 )
				{
					vec_token_ptr_push_back(&aux_postfix, cur_token);
					inserted = true;
					break;
				}

				// Get token from the top of auxilary stack
				aux_token_tmp = vec_token_ptr_get(&aux_postfix, aux_vec_len-1);
				if ( is_error(tmp_token) )
				{
					vec_token_ptr_destroy(&postfix_expr);
					vec_token_ptr_destroy(&aux_postfix);
					free(cur_token);
					forward_error(tmp_token, _Bool);
				}
				aux_token = *(aux_token_tmp._value);

				// if aux_atack.top == ) || aux_stack.top == operator with lower priority
				if ( aux_token->discriminant == openParen || isHigherPrior(cur_token->discriminant, aux_token->discriminant) )
				{
					vec_token_ptr_push_back(&aux_postfix, cur_token);
					inserted = true;
				}

				else
				{
					vec_token_ptr_pop_back(&aux_postfix);
					vec_token_ptr_push_back(&postfix_expr, aux_token);
				}

				aux_vec_len = vec_token_ptr_len(&aux_postfix);
			}

		}

		// (
		else if ( cur_token->discriminant == openParen )
		{
			if ( ! isInTokens(prev_token->discriminant, bef_openParen) )
			{
				Correct = false;
				break;
			}
			vec_token_ptr_push_back(&aux_postfix, cur_token);
		}

		// )
		else if ( cur_token->discriminant == closeParen )
		{
			aux_vec_len = vec_token_ptr_len(&aux_postfix);
			if ( ! isInTokens(prev_token->discriminant, bef_closeParen) || aux_vec_len == 0 )
			{
				Correct = false;
				break;
			}

			do
			{
				aux_token_tmp = vec_token_ptr_get(&aux_postfix, aux_vec_len-1);
				if ( is_error(tmp_token) )
				{
					vec_token_ptr_destroy(&postfix_expr);
					vec_token_ptr_destroy(&aux_postfix);
					free(cur_token);
					forward_error(tmp_token, _Bool);
				}
				aux_token = *(aux_token_tmp._value);

				vec_token_ptr_push_back(&postfix_expr, aux_token);
				vec_token_ptr_pop_back(&aux_postfix);
				aux_vec_len = vec_token_ptr_len(&aux_postfix);

			} while ( aux_vec_len != 0 && aux_token->discriminant != openParen );

			if ( aux_token->discriminant != openParen )
			{
				Correct = false;
				break;
			}
			else
			{
				vec_token_ptr_pop_back(&postfix_expr);
			}

		}

		// Get next token
		tmp_token = getToken();
		if ( is_error(tmp_token) )
		{
			vec_token_ptr_destroy(&postfix_expr);
			vec_token_ptr_destroy(&aux_postfix);
			forward_error(tmp_token, _Bool);
		}

		prev_token = cur_token;
		cur_token = tmp_token._value;
	}

	// Handle the last token
	// If expression is a if or while condition it has to end with )
	if ( !isInTokens(cur_token->discriminant, end_exp) || (cond_expr && prev_token->discriminant != closeParen) || !isInTokens(prev_token->discriminant, expr_last))
	{
		vec_token_ptr_destroy(&postfix_expr);
		vec_token_ptr_destroy(&aux_postfix);
		return_correct(false);
	}

	returnToken(cur_token);

	// Put the rest of the auxilary stack onto the postfix stack
	while ( vec_token_ptr_len(&aux_postfix) != 0 )
	{
		aux_token_tmp = vec_token_ptr_get(&aux_postfix, vec_token_ptr_len(&aux_postfix)-1);
		if ( is_error(tmp_token) )
		{
			vec_token_ptr_destroy(&postfix_expr);
			vec_token_ptr_destroy(&aux_postfix);
			free(cur_token);
			forward_error(tmp_token, _Bool);
		}
		aux_token = *(aux_token_tmp._value);

		if ( aux_token->discriminant == openParen )
		{
			Correct = false;
			break;
		}
		vec_token_ptr_push_back(&postfix_expr, aux_token);
		vec_token_ptr_pop_back(&aux_postfix);
	}

	// for (size_t i = 0; i < vec_token_ptr_len(&postfix_expr); i++ )
	// {
	// 	PT_PrintTokenType(postfix_expr.data[i]->discriminant);
	// }
	// putchar('\n');


	if ( Correct )
	{
		// Build postfix tree
		error(PT_Node_ptr) expr_tree = PT_FromPostFix(&postfix_expr);
		if ( is_error(expr_tree) )
		{
			vec_token_ptr_destroy(&aux_postfix);
			vec_token_ptr_destroy(&postfix_expr);
			forward_error(expr_tree, _Bool);
		}

		*token_node = expr_tree._value;
	}

	// PT_PrintExprTreeAsPostfix(*token_node);
	// putchar('\n');

	vec_token_ptr_destroy(&aux_postfix);
	vec_token_ptr_destroy(&postfix_expr);

	return_correct(Correct);
}


bool isInTokens ( tokenType Token, const tokenType * TokenList)
{
    if ( TokenList == NULL )
    {
        return false;
    }

    while ( *TokenList != N_VLD )
    {
        if ( Token == *(TokenList++) )
        {
            return true;
        }
    }

    return false;
}

error(none) skipTokens ( const tokenType * which, PT_Node_t* token_node )
{
    if ( which == NULL || token_node == NULL ) {return_error(INVALID_VAL, none);}

    error(token_ptr) tmp_token;
	error(PT_Node_ptr) tmp_node;
	PT_Data_t cur_node_data;
	PT_Node_t * cur_node = token_node;
	PT_Node_t * prev_node;

	do
	{
		tmp_token = getToken();
		if ( is_error(tmp_token) )
		{
			forward_error(tmp_token, none);
		}

		cur_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = tmp_token._value};
		tmp_node = PT_AddSibling(cur_node, cur_node_data);
		if ( is_error(tmp_node) )
		{
			forward_error(tmp_node, none);
		}
		prev_node = cur_node;
		cur_node = cur_node->rightSibling;

	} while ( isInTokens(tmp_token._value->discriminant, expr_tokens) );

    returnToken(cur_node->data.type.terminal);
	free(cur_node);
	prev_node->rightSibling = NULL;

    return_none();
}

error(_Bool) isNextToken( tokenType TokenName, PT_Node_t* token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	error(token_ptr) tmp_token = getToken();
	get_value(token_ptr, cur_token, tmp_token, _Bool);


	if ( cur_token->discriminant == TokenName )
	{
		PT_Data_t node_data = {.isTerminal = true, .type.terminal = cur_token};
		error(PT_Node_ptr) tmp_node = PT_AddSibling(token_node, node_data);
		if ( is_error(tmp_node) )
		{
			forward_error(tmp_node, _Bool);
		}

		return_value(true, _Bool);
	}

	else
	{
		returnToken(cur_token);
		return_value(false, _Bool);
	}
}

bool isHigherPrior ( tokenType first, tokenType second )
{
	static const tokenType mul_div_ops[] = { multiply, division, N_VLD };
	static const tokenType add_sub_cat_ops[] = { plusSign, minusSign, concatenation, N_VLD };
	static const tokenType rel_ops[] = { moreOper, moreOrEqOper, lessOper, lessOrEqOper, N_VLD };
	static const tokenType eq_ops[] = { EqOper, notEqOper, N_VLD };

	const unsigned mul_ops_idx = 0;
	const unsigned add_ops_idx = 1;
	const unsigned rel_ops_idx = 2;
	const unsigned eq_ops_idx = 3;

	// is higher priority
	static const bool precedence_rel[4][4] = { //mul  add   rel   eq
											{ false, true, true, true }, //mul
											{ false, false, true, true },//add
											{ false, false, false, true },//rel
											{ false, false, false, false }//eq
											};
	unsigned f_index = 0, s_index = 0;

	// Get the correct indices
	if ( isInTokens(first, mul_div_ops) ) { f_index = mul_ops_idx; }
	else if ( isInTokens(first, add_sub_cat_ops) ) {f_index = add_ops_idx; }
	else if ( isInTokens(first, rel_ops) ) {f_index = rel_ops_idx; }
	else if ( isInTokens(first, eq_ops) ) {f_index = eq_ops_idx;}

	if ( isInTokens(second, mul_div_ops) ) { s_index = mul_ops_idx; }
	else if ( isInTokens(second, add_sub_cat_ops) ) {s_index = add_ops_idx; }
	else if ( isInTokens(second, rel_ops) ) {s_index = rel_ops_idx; }
	else if ( isInTokens(second, eq_ops) ) {s_index = eq_ops_idx;}

	return precedence_rel[f_index][s_index];
}
