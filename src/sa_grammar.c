/**
 * @file sa_grammar.c
 * @author Samuel Stolarik
 * @brief Implementation of grammar rules and token checks
 *
 * @date 2022-11-08
 */

#include "./headers/IFJ_2022.h"
#include "./headers/syntactic_analysis.h"
#include "./headers/sa_private.h"
#include "./headers/scaner.h"
#include "./headers/error.h"

#include <stdbool.h>

#define push_token( vec, token ) do {vec_token_ptr_push_back((vec), (token));} while(0)
#define error_test( er_obj, er_type ) do { if(is_error(er_obj)) {forward_error(er_obj, er_type);}} while(0)
#define error_test_destroy( er_obj, er_type, vec ) do {if (is_error(er_obj)) { vec_token_ptr_destroy(vec); forward_error(er_obj, er_type); }} while(0)
#define test_result(result) if ( ! result )                         \
                            {                                       \
                                vec_token_ptr_destroy(&m_tokens);   \
                                return_value(false, _Bool);         \
                            }                                       \


const tokenType expr_tokens[] = { identOfVar, integer, decNum, string, plusSign, minusSign,
                            multiply, division, concatenation, lessOper, lessOrEqOper,
                            moreOper, moreOrEqOper, EqOper, notEqOper, openParen,
                            closeParen, N_VLD };

//Rules

//<PROG>

error( _Bool ) sa_prog_fdef__ ()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // IdentOfFunc
    result = sa_is_token__(identOfFunct);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // Open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <PARAMS>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_Params(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // Close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // Colon
    result = sa_is_token__(colon);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <TYPE>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // Open Set Paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <ST_LIST>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_ST_List(cur_token._value);
    error_test_destroy(cur_token, _Bool, &m_tokens);

    test_result(result._value);

    // Close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    return_value(true, _Bool);
}

error( _Bool ) sa_prog_if__ ()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // If token
    result = sa_is_token__(ifT);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <ARG_TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_ARG_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // open set paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <BODY>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // Close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // else token
    result = sa_is_token__(elseT);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // open set paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <Body>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    return_value(true, _Bool);
}

error( _Bool ) sa_prog_while__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <ARG_TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_ARG_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // open set paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <BODY>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(cur_token, _Bool, &m_tokens);
    test_result(result._value);

    // close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);


    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_prog_assign__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // = token
    result = sa_is_token__(EqOper);
    error_test(result, _Bool);
    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <EXPR>
    //TODO expression analysis
    skipExpr(expr_tokens);

    // semicolon
    result = sa_is_token__(semicolon);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);


    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_prog_return__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <RETVAL>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_RetVal(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // semicolon
    result = sa_is_token__(semicolon);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);


    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_prog_fcall__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);
    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    //<ARGS>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_Args(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //semicolon
    result = sa_is_token__(semicolon);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);


    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_prog_expr__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    //TODO <EXPR>
    error(none) tmp = skipExpr(expr_tokens);
    error_test(tmp, _Bool);

    // semicolon
    result = sa_is_token__(semicolon);
    error_test(result, _Bool);
    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <PROG>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Prog(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);


    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);

}

//<BODY>

error( _Bool ) sa_body_stat__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <STATEMENT>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_Statement(cur_token._value);
    error_test_destroy(cur_token, _Bool, &m_tokens);
    test_result(result._value);

    // <BODY>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(cur_token, _Bool, &m_tokens);
    test_result(cur_token._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_body_empty__()
{
    return_value(true, _Bool);
}



//<STATEMENT>

error( _Bool ) sa_stat_if__()
{
error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // If token
    result = sa_is_token__(ifT);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <ARG_TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_ARG_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // open set paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <BODY>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // Close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // else token
    result = sa_is_token__(elseT);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // open set paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // <Body>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    // close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);

    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    return_value(true, _Bool);

}

error( _Bool ) sa_stat_while__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);

    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <ARG_TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_ARG_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // open set paren
    result = sa_is_token__(openSetParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <BODY>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_Body(cur_token._value);
    error_test_destroy(cur_token, _Bool, &m_tokens);
    test_result(result._value);

    // close set paren
    result = sa_is_token__(closeSetParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);

}

error( _Bool ) sa_stat__assign__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // = token
    result = sa_is_token__(EqOper);
    error_test(result, _Bool);
    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    // <EXPR>
    //TODO expression analysis
    skipExpr(expr_tokens);

    // semicolon
    result = sa_is_token__(semicolon);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);

}

error( _Bool ) sa_stat_return__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <RETVAL>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_RetVal(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // semicolon
    result = sa_is_token__(semicolon);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);

}

error( _Bool ) sa_stat_fcall__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // open paren
    result = sa_is_token__(openParen);
    error_test(result, _Bool);
    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    //<ARGS>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_Args(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // close paren
    result = sa_is_token__(closeParen);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //semicolon
    result = sa_is_token__(semicolon);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);

}

error ( _Bool ) sa_stat_expr__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    //TODO <EXPR>
    error(none) tmp = skipExpr(expr_tokens);
    error_test(tmp, _Bool);

    // semicolon
    result = sa_is_token__(semicolon);
    error_test(result, _Bool);
    if ( ! result._value )
    {
        return_value(false, _Bool);
    }

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);

}

//<PARAMS>

error( _Bool ) sa_params__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // identofvar
    result = sa_is_token__(identOfVar);
    test_result(result._value);

    // <PARAMS_NEXT>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_ParamsNext(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't free tokens if they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    return_value(true, _Bool);

}

error( _Bool ) sa_params_empty__()
{
    return_value(true, _Bool);
}



//<PARAMS_NEXT>

error( _Bool ) sa_params_next__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // identofvar
    result = sa_is_token__(identOfVar);
    test_result(result._value);

    // <PARAMS_NEXT>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_ParamsNext(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);


    //TODO don't free tokens if they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    return_value(true, _Bool);

}

error( _Bool ) sa_params_next_empty__()
{
    return_value(true, _Bool);
}



//<ARGS>

error( _Bool ) sa_args__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <ARG_TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result = SA_ARG_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <ARGS_NEXT>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_ArgsNext(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_args_empty__()
{
    return_value(true, _Bool);
}



//<ARGS_NEXT>

error( _Bool ) sa_args_next__()
{
    error(_Bool) result;
    error(token_ptr) cur_token;
    vec_token_ptr m_tokens = new_vec_token_ptr();

    // <ARG_TYPE>
    cur_token = getToken();
    error_test(cur_token, _Bool);
    push_token(&m_tokens, cur_token._value);

    result= SA_ARG_Type(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    // <ARGS_NEXT>
    cur_token = getToken();
    error_test_destroy(cur_token, _Bool, &m_tokens);
    push_token(&m_tokens, cur_token._value);

    result = SA_ArgsNext(cur_token._value);
    error_test_destroy(result, _Bool, &m_tokens);
    test_result(result._value);

    //TODO don't delete tokens in case they are in the prog tree
    vec_token_ptr_destroy(&m_tokens);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_args_next_empty__()
{
    return_value(true, _Bool);
}



//<ARG_TYPE>
error( _Bool ) sa_arg_type_term__()
{
    error(token_ptr) cur_token = getToken();
    error_test(cur_token, _Bool);
    bool result = false;

    const tokenType tokenList_term[] = {nullT, decNum, integer, string, N_VLD};

    if ( isInTokens(cur_token._value->discriminant, tokenList_term) )
    {
        result = true;
    }

    //TODO put token into prog tree instead
    free(cur_token._value);

    return_value(result, _Bool);
}

error( _Bool ) sa_arg_type_var__()
{
    error(_Bool) result = sa_is_token__(identOfVar);
    error_test(result, _Bool);

    return_value(result._value, _Bool);
}



//<RVAL>

error( _Bool ) sa_rval_term__()
{

}

error( _Bool ) sa_rval_func__()
{

}

error( _Bool ) sa_rval_expr__()
{

}



//<RETVAL>

error( _Bool ) sa_retval__()
{
    //TODO <EXPR>
    error(none) tmp = skipExpr(expr_tokens);
    error_test(tmp, _Bool);

    //Return true
    return_value(true, _Bool);
}

error( _Bool ) sa_retval_empty__()
{
    return_value(true, _Bool);
}



//<TERM>

error( _Bool ) sa_term__()
{
    return_value(true, _Bool);
}



//<TYPE>

error( _Bool ) sa_type__()
{
    return_value(true, _Bool);
}

error( _Bool ) sa_type_n__()
{
    return_value(true,_Bool);
}



//Terminals

error( _Bool ) sa_is_token__( tokenType token_name )
{
    bool Correct = false;
    error(token_ptr) token = getToken();

    if ( is_error(token) )
    {
        forward_error(token, _Bool);
    }

    //Check for identOfFunc and the rest
    if ( token._value->discriminant == token_name )
    {
        Correct = true;
    }

    //TODO insert into prog tree instead of freeing
    free(token._value);

    return_value(Correct, _Bool );
}

