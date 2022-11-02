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

//Rules

//<PROG>

error( _Bool ) sa_prog_fdef__ ()
{

}

error( _Bool ) sa_prog_if__ ()
{

}

error( _Bool ) sa_prog_while__()
{

}

error( _Bool ) sa_prog_assign__()
{

}

error( _Bool ) sa_prog_return__()
{

}

error( _Bool ) sa_prog_rval__()
{

}


//<BODY>

error( _Bool ) sa_body_stat__()
{

}

error( _Bool ) sa_body_empty__()
{

}



//<STATEMENT>

error( _Bool ) sa_stat_if__()
{

}

error( _Bool ) sa_stat_while__()
{

}

error( _Bool ) sa_stat_rval__()
{

}

error( _Bool ) sa_stat__assign__()
{

}

error( _Bool ) sa_stat_return__()
{

}


//<PARAMS>

error( _Bool ) sa_params__()
{

}

error( _Bool ) sa_params_empty__()
{

}



//<PARAMS_NEXT>

error( _Bool ) sa_params_next__()
{

}

error( _Bool ) sa_params_next_empty__()
{

}



//<ARGS>

error( _Bool ) sa_args__()
{

}

error( _Bool ) sa_args_empty__()
{

}



//<ARGS_NEXT>

error( _Bool ) sa_args_next__()
{

}

error( _Bool ) sa_args_next_empty__()
{

}



//<ARG_TYPE>
error( _Bool ) sa_arg_type_term__()
{

}

error( _Bool ) sa_arg_type_var__()
{

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

}

error( _Bool ) sa_retval_empty__()
{

}



//<TERM>

error( _Bool ) sa_term__()
{

}



//<TYPE>

error( _Bool ) sa_type__()
{

}

error( _Bool ) sa_type_n__()
{

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

