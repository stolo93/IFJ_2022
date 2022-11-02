/**
 * @file sa_private.h
 * @author Samuel Stolarik xstola03@stud.fit.vutbr.cz
 * @brief Private functions used in syntactic analysis
 * @date 2022-11-08
 */

#ifndef SA_PRIVATE
#define SA_PRIVATE

#include "./IFJ_2022.h"
#include "./error.h"

//Private function prototypes
//Rules

//<PROG>

error( _Bool ) sa_prog_fdef__ ();
error( _Bool ) sa_prog_if__ ();
error( _Bool ) sa_prog_while__();
error( _Bool ) sa_prog_assign__();
error( _Bool ) sa_prog_fcall__();
error( _Bool ) sa_prog_return__();
error( _Bool ) sa_prog_expr__();

//<BODY>

error( _Bool ) sa_body_stat__();
error( _Bool ) sa_body_empty__();


//<STATEMENT>

error( _Bool ) sa_stat_if__();
error( _Bool ) sa_stat_while__();
error( _Bool ) sa_stat_fcall__();
error( _Bool ) sa_stat__assign__();
error( _Bool ) sa_stat_return__();
error( _Bool ) sa_stat_expr__();

//<PARAMS>

error( _Bool ) sa_params__();
error( _Bool ) sa_params_empty__();


//<PARAMS_NEXT>

error( _Bool ) sa_params_next__();
error( _Bool ) sa_params_next_empty__();


//<ARGS>

error( _Bool ) sa_args__();
error( _Bool ) sa_args_empty__();


//<ARGS_NEXT>

error( _Bool ) sa_args_next__();
error( _Bool ) sa_args_next_empty__();


//<ARG_TYPE>
error( _Bool ) sa_arg_type_term__();
error( _Bool ) sa_arg_type_var__();


//<RETVAL>

error( _Bool ) sa_retval__();
error( _Bool ) sa_retval_empty__();


//<TERM>

error( _Bool ) sa_term__();


//<TYPE>

error( _Bool ) sa_type__();
error( _Bool ) sa_type_n__();


//Terminals

error( _Bool ) sa_is_token__( tokenType token_name );

#endif
