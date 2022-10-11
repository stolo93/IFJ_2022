##### Grammar for IFJ_2022 language
###### Program start
<PROLOG> -> prolog <PROG>
<PROG> -> <ST_LIST> endOfFile

###### Statements
<ST_LIST> -> <STATEMENT> <STLIST>
<ST_LIST> ->''

<STATEMENT> -> if (<RVAL>) {<ST_LIST>} else {<ST_LIST>}
<STATEMENT> -> while (<RVAL>) {<ST_LIST>}
<STATEMENT> -> <RVAL>;
<STATEMENT> -> identOfVar = <RVAL>;
<STATEMENT> -> return <RET_VAL>;
<STATEMENT> -> <FUN_DEF>

###### Functions
<FCAL> -> identOfFunc(<ARGS>)
<FDEF> -> function identOfFunc(<PARAMS>) : <TYPE> {<ST_LIST>}

###### Function parameters
<PARAMS> -> <TYPE> identOfVar <PARAMS_NEXT>
<PARAMS> ->''

<PARAMS_NEXT> ->, <TYPE> identOfVar <PARAMS_NEXT>
<PARAMS_NEXT> ->''

<ARGS> -> <RVAL> <ARGS_NEXT>
<ARGS> ->''

<ARGS_NEXT> ->, <RVAL> <ARGS_NEXT>
<ARGS_NEXT> ->''

###### Variables and values
<RVAL> -> <TERM>
<RVAL> -> <FCAL>
<RVAL> -> <EXPR>

<RET_VAL> -> <RVAL>
<RET_VAL> ->''

<TERM> -> null
<TERM> -> float_lit
<TERM> -> int_lit
<TERM> -> string_lit

###### Types
<TYPE> -> identOfType
<TYPE> -> identOfTypeN
