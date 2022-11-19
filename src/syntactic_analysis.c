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

#define test_result(result) do{ if( !result ) { return_value(false, _Bool); } } while(0)
#define test_error(er_obj, forw_type) do{ if( is_error(er_obj) ) { forward_error(er_obj, forw_type)} } while(0)

const tokenType expr_tokens[] = {
								identOfVar, integer, decNum, string, plusSign, minusSign,
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
		get_value(PT_Node_ptr, prog_child_node, tmp_node, _Bool);

		error(_Bool) tmp_result = SA_Prog(&prog_child_node);
		get_value(bool, res_sa_prog, tmp_result, _Bool);

		Correct = res_sa_prog;
	}

    return_value(Correct, _Bool);
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
			get_value(PT_Node_ptr, params_node_child_fdef, tmp_node, _Bool);

			tmp_result = SA_Params(&params_node_child_fdef);
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
			get_value(PT_Node_ptr, type_node_child_fdef, tmp_node, _Bool);

			tmp_result = SA_Type(type_node_child_fdef);
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
			get_value(PT_Node_ptr, body_node_child_fdef, tmp_node, _Bool);


			tmp_result = SA_Body(&body_node_child_fdef);
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
			get_value(PT_Node_ptr, prog_node_child_fdef, tmp_node, _Bool);


			tmp_result = SA_Prog(&prog_node_child_fdef);
			get_value(bool, res_sa_prog_iof, tmp_result, _Bool);
			test_result(res_sa_prog_iof);

			// If everything got this far it's correct
			Correct = true;
            break;

        // If else statement
        case ifT:

			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_if, tmp_result, _Bool);
			test_result(is_open_paren_if);
			cur_node = cur_node->rightSibling;

			// Create node <ARG_TYPE>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARG_TYPE};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_if, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = arg_type_token_if};
			tmp_node = PT_AddChild(arg_type_node_if, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_child_if, tmp_node, _Bool);

			tmp_result = SA_ARG_Type(&arg_type_node_child_if);
			get_value(bool, res_sa_arg_type_if, tmp_result, _Bool);
			test_result(res_sa_arg_type_if);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_if, tmp_result, _Bool);
			test_result(is_close_paren_if);
			cur_node = cur_node->rightSibling;

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
			get_value(PT_Node_ptr, body_node_child_if1, tmp_node, _Bool);

			tmp_result = SA_Body(&body_node_child_if1);
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
			get_value(PT_Node_ptr, body_node_child_if2,tmp_node, _Bool);

			tmp_result = SA_Body(&body_node_child_if2);
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
			get_value(PT_Node_ptr, prog_node_child_if, tmp_node, _Bool);

			tmp_result = SA_Prog(&prog_node_child_if);
			get_value(bool, res_sa_prog_if, tmp_result, _Bool);
			test_result(res_sa_prog_if);

			// End of rule
			Correct = true;
            break;

        // While
        case whileT:

			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_while, tmp_result, _Bool);
			test_result(is_open_paren_while);
			cur_node = cur_node->rightSibling;

			// Create node for <ARG_TYPE>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARG_TYPE};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = arg_type_token_while};
			tmp_node = PT_AddChild(arg_type_node_while, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_child_while, tmp_node, _Bool);

			tmp_result = SA_ARG_Type(&arg_type_node_child_while);
			get_value(bool, res_sa_arg_type_while, tmp_result, _Bool);
			test_result(res_sa_arg_type_while);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_while, tmp_result, _Bool);
			test_result(is_close_paren_while);
			cur_node = cur_node->rightSibling;

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
			get_value(PT_Node_ptr, body_node_child_while, tmp_node, _Bool);

			tmp_result = SA_Body(&body_node_child_while);
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
			get_value(PT_Node_ptr, prog_node_child_while, tmp_node, _Bool);

			tmp_result = SA_Prog(&prog_node_child_while);
			get_value(bool, res_sa_prog_while, tmp_result, _Bool);
			test_result(res_sa_prog_while);

			//End of rule while...
			Correct = true;
            break;

        // Function call
        case identOfFunct:

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
			get_value(PT_Node_ptr, args_node_child_fcal, tmp_node, _Bool);

			tmp_result = SA_Args(&args_node_child_fcal);
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
			get_value(PT_Node_ptr, prog_node_child_fcal, tmp_node, _Bool);

			tmp_result = SA_Prog(&prog_node_child_fcal);
			get_value(bool, res_sa_prog_fcal, tmp_result, _Bool);
			test_result(res_sa_prog_fcal);

			// End of rule function call
			Correct = true;
            break;

        // Assignement
        case identOfVar:

			//eqOper
			tmp_result = isNextToken(EqOper, cur_node);
			get_value(bool, is_eq_assign, tmp_result, _Bool);
			test_result(is_eq_assign);
			cur_node = cur_node->rightSibling;

			// Create node for <EXPR>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = EXPR};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_assign, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_assign, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = expr_token_assign};
			tmp_node = PT_AddChild(expr_node_assign, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_child_assign, tmp_node, _Bool);

			tmp_result = SA_Expr(expr_node_child_assign);
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
			get_value(PT_Node_ptr, prog_node_child_assign, tmp_node, _Bool);

			tmp_result = SA_Prog(&prog_node_child_assign);
			get_value(bool, res_sa_prog_assign, tmp_result, _Bool);
			test_result(res_sa_prog_assign);

			// End of assignment rule
			Correct = true;
            break;

        //Return statement
        case returnT:

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
			get_value(PT_Node_ptr, ret_val_node_child_ret, tmp_node, _Bool);

			tmp_result = SA_RetVal(&ret_val_node_child_ret);
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
			get_value(PT_Node_ptr, prog_node_child_ret, tmp_node, _Bool);

			tmp_result = SA_Prog(&prog_node_child_ret);
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
            if ( isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens) )
            {
				// Create <EXPR> node and insert it as a parent of the @p token_node
				PT_Data_t expr_node_data = {.isTerminal = false, .type.nonTerminal = EXPR};
				error(PT_Node_ptr) tmp_node = PT_CreateNode(expr_node_data);
				get_value(PT_Node_ptr, expr_node, tmp_node, _Bool);

				expr_node->leftChild = *token_node;
				*token_node = expr_node;

                tmp_result = SA_Expr(expr_node->leftChild);
				get_value(bool, res_sa_expr, tmp_result, _Bool);
				test_result(res_sa_expr);

				Correct = true;
            }
            break;
    }

    return_value(Correct, _Bool);
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

    const tokenType tokenList_statement[]   = { identOfFunct, ifT, whileT, identOfVar, returnT, N_VLD};
    const tokenType tokenList_eps[]         = { closeSetParen , N_VLD};

    //Epsilon rule
    if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_eps) )
    {
        Correct = true;
		returnToken((*token_node)->data.type.terminal);
		*token_node = NULL;
    }

    //Statement
    else if ( isInTokens((*token_node)->data.type.terminal->discriminant, tokenList_statement) || isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens))
    {
		// Create <STATEMENT> node and insert it as a parent of current token node
		PT_Data_t stat_node_data = {.isTerminal = false, .type.nonTerminal = STATEMENT};
		error(PT_Node_ptr) tmp_node = PT_CreateNode(stat_node_data);
		get_value(PT_Node_ptr, stat_node, tmp_node, _Bool);

		stat_node->leftChild = *token_node;
		*token_node = stat_node;

		error(_Bool) result = SA_Statement(&(stat_node->leftChild));
		get_value(bool, res_sa_statement, result, _Bool);
		test_result(res_sa_statement);

		Correct = true;
    }

    return_value(Correct, _Bool);
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
			get_value(PT_Node_ptr, args_node_child_fcal, tmp_node, _Bool);

			tmp_result = SA_Args(&args_node_child_fcal);
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


        // If Statement
        case ifT:

			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_if, tmp_result, _Bool);
			test_result(is_open_paren_if);
			cur_node = cur_node->rightSibling;

			// Create node <ARG_TYPE>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARG_TYPE};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_if, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_if, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = arg_type_token_if};
			tmp_node = PT_AddChild(arg_type_node_if, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_child_if, tmp_node, _Bool);

			tmp_result = SA_ARG_Type(&arg_type_node_child_if);
			get_value(bool, res_sa_arg_type_if, tmp_result, _Bool);
			test_result(res_sa_arg_type_if);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_if, tmp_result, _Bool);
			test_result(is_close_paren_if);
			cur_node = cur_node->rightSibling;

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
			get_value(PT_Node_ptr, body_node_child_if1, tmp_node, _Bool);

			tmp_result = SA_Body(&body_node_child_if1);
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
			get_value(PT_Node_ptr, body_node_child_if2,tmp_node, _Bool);

			tmp_result = SA_Body(&body_node_child_if2);
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

        // While
        case whileT:

			// Open Paren
			tmp_result = isNextToken(openParen, cur_node);
			get_value(bool, is_open_paren_while, tmp_result, _Bool);
			test_result(is_open_paren_while);
			cur_node = cur_node->rightSibling;

			// Create node for <ARG_TYPE>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = ARG_TYPE};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_while, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <ARG_TYPE>
			tmp_token = getToken();
			get_value(token_ptr, arg_type_token_while, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = arg_type_token_while};
			tmp_node = PT_AddChild(arg_type_node_while, tmp_node_data);
			get_value(PT_Node_ptr, arg_type_node_child_while, tmp_node, _Bool);

			tmp_result = SA_ARG_Type(&arg_type_node_child_while);
			get_value(bool, res_sa_arg_type_while, tmp_result, _Bool);
			test_result(res_sa_arg_type_while);

			// Close paren
			tmp_result = isNextToken(closeParen, cur_node);
			get_value(bool, is_close_paren_while, tmp_result, _Bool);
			test_result(is_close_paren_while);
			cur_node = cur_node->rightSibling;

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
			get_value(PT_Node_ptr, body_node_child_while, tmp_node, _Bool);

			tmp_result = SA_Body(&body_node_child_while);
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

        // Assignment
        case identOfVar:

			//eqOper
			tmp_result = isNextToken(EqOper, cur_node);
			get_value(bool, is_eq_assign, tmp_result, _Bool);
			test_result(is_eq_assign);
			cur_node = cur_node->rightSibling;

			// Create node for <EXPR>
			tmp_node_data = (PT_Data_t) {.isTerminal = false, .type.nonTerminal = EXPR};
			tmp_node = PT_AddSibling(cur_node, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_assign, tmp_node, _Bool);
			cur_node = cur_node->rightSibling;

			// <EXPR>
			tmp_token = getToken();
			get_value(token_ptr, expr_token_assign, tmp_token, _Bool);
			tmp_node_data = (PT_Data_t) {.isTerminal = true, .type.terminal = expr_token_assign};
			tmp_node = PT_AddChild(expr_node_assign, tmp_node_data);
			get_value(PT_Node_ptr, expr_node_child_assign, tmp_node, _Bool);

			tmp_result = SA_Expr(expr_node_child_assign);
			get_value(bool, res_sa_expr, tmp_result, _Bool);
			test_result(res_sa_expr);

			// Semicolon
			tmp_result = isNextToken(semicolon, cur_node);
			get_value(bool, is_semicolon_assign, tmp_result, _Bool);
			test_result(is_semicolon_assign);
			cur_node = cur_node->rightSibling;

			// End of assignment rule
			Correct = true;
            break;

        // Return statement
        case returnT:

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
			get_value(PT_Node_ptr, ret_val_node_child_ret, tmp_node, _Bool);

			tmp_result = SA_RetVal(&ret_val_node_child_ret);
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


        //Expresion
        default:
            if ( isInTokens((*token_node)->data.type.terminal->discriminant, expr_tokens) )
            {
				// Create <EXPR> node and insert it as a parent of the @p token_node
				PT_Data_t expr_node_data = {.isTerminal = false, .type.nonTerminal = EXPR};
				error(PT_Node_ptr) tmp_node = PT_CreateNode(expr_node_data);
				get_value(PT_Node_ptr, expr_node, tmp_node, _Bool);

				expr_node->leftChild = *token_node;
				*token_node = expr_node;

                tmp_result = SA_Expr(expr_node->leftChild);
				get_value(bool, res_sa_expr, tmp_result, _Bool);
				test_result(res_sa_expr);

				Correct = true;
            }
            break;
    }

    return_value(Correct, _Bool);

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
		get_value(PT_Node_ptr, args_node_child_fcal, tmp_node, _Bool);

		tmp_result = SA_Args(&args_node_child_fcal);
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

		error(_Bool) result = SA_Expr(expr_node->leftChild);
		get_value(bool, res_sa_expr, result, _Bool);
		test_result(res_sa_expr);

		Correct = true;
	}

	return_value(Correct, _Bool);
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

		error(_Bool) result = SA_Expr(expr_node->leftChild);
		get_value(bool, res_sa_expr, result, _Bool);
		test_result(res_sa_expr);

		Correct = true;
    }

    return_value(Correct, _Bool);
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
    const tokenType tokenList_arg_type[] = { identOfVar, nullT, decNum, integer, string, N_VLD };

    // Epsilon
    if ( (*token_node)->data.type.terminal->discriminant == closeParen )
    {
		returnToken((*token_node)->data.type.terminal);
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
		get_value(PT_Node_ptr, args_next_child_node, tmp_node, _Bool);

		tmp_result = SA_ArgsNext(&args_next_child_node);
		get_value(bool, res_sa_args_next, tmp_result, _Bool);
		test_result(res_sa_args_next);

		Correct = true;
    }

    return_value(Correct, _Bool);
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
			get_value(PT_Node_ptr, arg_type_child_node, tmp_node, _Bool);

			tmp_result = SA_ARG_Type(&arg_type_child_node);
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
			get_value(PT_Node_ptr, args_next_child_node, tmp_node, _Bool);

			tmp_result = SA_ArgsNext(&args_next_child_node);
			get_value(bool, res_sa_args_next, tmp_result, _Bool);
			test_result(res_sa_args_next);

			Correct = true;
            break;

		//EPSILON
        case closeParen:
			Correct = true;
			returnToken((*token_node)->data.type.terminal);
			*token_node = NULL;
            break;

		default:
			break;
    }

    return_value(Correct, _Bool);
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
    const tokenType tokenList_term[] = {nullT, decNum, integer, string, N_VLD};

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

    return_value(Correct, _Bool);
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
		get_value(PT_Node_ptr, params_next_child_node, tmp_node, _Bool);

		tmp_result = SA_ParamsNext(&params_next_child_node);
		get_value(bool, res_sa_params_next, tmp_result, _Bool);
		test_result(res_sa_params_next);

		Correct = true;
	}

	return_value(Correct, _Bool);

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
		*token_node = NULL;
	}

	// , <TYPE>...
	else if ( (*token_node)->data.type.terminal->discriminant == colon )
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
		get_value(PT_Node_ptr, type_child_node, tmp_node, _Bool);

		tmp_result = SA_Type(type_child_node);
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
		get_value(PT_Node_ptr, params_next_child_node, tmp_node, _Bool);

		tmp_result = SA_ParamsNext(&params_next_child_node);
		get_value(bool, res_sa_params_next, tmp_result, _Bool);
		test_result(res_sa_params_next);

		Correct = true;
	}

	return_value(Correct, _Bool);
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

	return_value(Correct, _Bool);
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

	return_value(Correct, _Bool);
}

error( _Bool ) SA_Expr ( PT_Node_t * token_node )
{
	if ( token_node == NULL )
	{
		return_error(INVALID_VAL, _Bool);
	}

	bool Correct = false;

	//TODO implement proper expression analysis
	error(none) ret_val = skipTokens(expr_tokens, token_node);
	if ( is_error(ret_val) )
	{
		forward_error(ret_val, _Bool);
	}
	Correct = true;

	return_value(Correct, _Bool);
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
