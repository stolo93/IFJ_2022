#include "headers/syntax_tree.h"
#include "headers/error.h"
#include "headers/IFJ_2022.h"
#include "headers/vector.h"
#include "headers/symtable.h"
#include "headers/scaner.h"
#include "headers/semantic_analysis.h"

#define LEFT 0
#define RIGHT 1

extern vec_htab_t_ptr symtable_vector;

/*
  getting down to operands DONE
  extracting oprands dType DONE
  checking left and right child DONE
  checking if one child is float DONE
  adding conversion node DONE
  concatenation check DONE
  for normal operation if operand istring retuern error DONE
  if it is division return float DONE
  relation operations TODO

  */

error( dType ) getTerminalDType( PT_Node_ptr node)
{
    if( node == NULL )
    {
        return_error( ERROR_SEM_NODE , dType ); //node is null pointer
    }
    tokenType disc = node->data.type.terminal->discriminant;
    if(disc == identOfVar || disc == identOfFunct)
    {
        for( size_t counter = 0 ; counter < symtable_vector.len__ ; counter++ )
        {
            get_value( htab_pair_t_ptr , tmp , htab_find( symtable_vector.data[counter] , node->data.type.terminal->info.string ) , dType);

            if( tmp == NULL && counter == symtable_vector.len__ - 1)
            {
                return_error(ERROR_SEM_EXPR , dType );//nonexistent varaible or function
            }
            if( tmp != NULL )
            {
                return_value( tmp->diff.var.dataType , dType);
            }
        }
    }
    if( disc == integer )
    {
        return_value( integerT , dType);
    }
    if( disc == decNum )
    {
        return_value( floatingT , dType );
    }
    if( disc == string )
    {
        return_value( stringT , dType );
    }
    if( disc == nullT )
    {
        return_value( noType , dType );
    }
    return_value( notDefined , dType );
}

error( none ) addIntToRealNode( PT_Node_ptr node , bool side)
{
    if( node == NULL)
    {
        return_error( ERROR_SEM_NODE , none );
    }
    PT_Node_ptr tmp = NULL;
    PT_Node_ptr new = NULL;
    token_ptr newTerminal = ( token_ptr ) malloc( sizeof( token_t ));

    if( newTerminal == NULL)
    {
        return_error( ERROR_MAL , none);
    }
    newTerminal->discriminant = intToReal;
    newTerminal->info.integer = 0;

    PT_Data_t data = {.isTerminal = true , .type.terminal = newTerminal};
    if( side == true)
    {
        tmp = node->rightSibling;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->rightSibling = NULL;
        new->leftChild = tmp;
        node->rightSibling = new;

    }
    else
    {
       tmp = node->leftChild;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->leftChild = tmp;
        new->rightSibling = NULL;
        node->leftChild = new; 
    }
    return_none();
}

error( none ) addNullToIntNode (PT_Node_ptr node , bool side)
{
        if( node == NULL)
    {
        return_error( ERROR_SEM_NODE , none );
    }
    PT_Node_ptr tmp = NULL;
    PT_Node_ptr new = NULL;
    token_ptr newTerminal = ( token_ptr ) malloc( sizeof( token_t ));

    if( newTerminal == NULL)
    {
        return_error( ERROR_MAL , none);
    }
    newTerminal->discriminant = nullToInt;
    newTerminal->info.integer = 0;

    PT_Data_t data = {.isTerminal = true , .type.terminal = newTerminal};
    if( side == true)
    {
        tmp = node->rightSibling;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->rightSibling = NULL;
        new->leftChild = tmp;
        node->rightSibling = new;

    }
    else
    {
       tmp = node->leftChild;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->leftChild = tmp;
        new->rightSibling = NULL;
        node->leftChild = new; 
    }
    return_none();
}

error( none ) addNullToRealNode(PT_Node_ptr node , bool side)
{
        if( node == NULL)
    {
        return_error( ERROR_SEM_NODE , none );
    }
    PT_Node_ptr tmp = NULL;
    PT_Node_ptr new = NULL;
    token_ptr newTerminal = ( token_ptr ) malloc( sizeof( token_t ));

    if( newTerminal == NULL)
    {
        return_error( ERROR_MAL , none);
    }
    newTerminal->discriminant = nullToReal;
    newTerminal->info.integer = 0;

    PT_Data_t data = {.isTerminal = true , .type.terminal = newTerminal};
    if( side == true)
    {
        tmp = node->rightSibling;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->rightSibling = NULL;
        new->leftChild = tmp;
        node->rightSibling = new;

    }
    else
    {
       tmp = node->leftChild;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->leftChild = tmp;
        new->rightSibling = NULL;
        node->leftChild = new; 
    }
    return_none();
}

error( none ) addNullToBoolNode(PT_Node_ptr node , bool side)
{
        if( node == NULL)
    {
        return_error( ERROR_SEM_NODE , none );
    }
    PT_Node_ptr tmp = NULL;
    PT_Node_ptr new = NULL;
    token_ptr newTerminal = ( token_ptr ) malloc( sizeof( token_t ));

    if( newTerminal == NULL)
    {
        return_error( ERROR_MAL , none);
    }
    newTerminal->discriminant = nullToBool;
    newTerminal->info.integer = 0;

    PT_Data_t data = {.isTerminal = true , .type.terminal = newTerminal};
    if( side == true)
    {
        tmp = node->rightSibling;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->rightSibling = NULL;
        new->leftChild = tmp;
        node->rightSibling = new;

    }
    else
    {
       tmp = node->leftChild;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->leftChild = tmp;
        new->rightSibling = NULL;
        node->leftChild = new; 
    }
    return_none();
}

error( none ) addNullToStringNode(PT_Node_ptr node , bool side)
{
        if( node == NULL)
    {
        return_error( ERROR_SEM_NODE , none );
    }
    PT_Node_ptr tmp = NULL;
    PT_Node_ptr new = NULL;
    token_ptr newTerminal = ( token_ptr ) malloc( sizeof( token_t ));

    if( newTerminal == NULL)
    {
        return_error( ERROR_MAL , none);
    }
    newTerminal->discriminant = nullToString;
    newTerminal->info.integer = 0;

    PT_Data_t data = {.isTerminal = true , .type.terminal = newTerminal};
    if( side == true)
    {
        tmp = node->rightSibling;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->rightSibling = NULL;
        new->leftChild = tmp;
        node->rightSibling = new;

    }
    else
    {
       tmp = node->leftChild;
        error( PT_Node_ptr ) newNode = PT_CreateNode( data );
        if( is_error( newNode ))
        {
            forward_error( newNode , none );
        }
        new = newNode._value;
        new->leftChild = tmp;
        new->rightSibling = NULL;
        node->leftChild = new; 
    }
    return_none();
}

error( none ) operandConversion( PT_Node_ptr node , dType type1 , dType type2)
{
    if( node == NULL)
    {
        return_error( ERROR_SEM_NODE , none );
    }
    if( type1 == floatingT && type2 == integerT)
    {
        error( none ) success = addIntToRealNode( node , RIGHT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if( type1 == integerT && type2 == floatingT )
    {
        error( none ) success = addIntToRealNode( node , LEFT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none();
    }
    else if( ( type1 == stringT && type2 != stringT ) || ( type2 == stringT && type1 != stringT))
    {
        return_error( ERROR_SEM_TYPE , none); //string can't be converted to any data type
    }
    else if( type1 == integerT && type2 == integerT)
    {
        error( none ) success = addIntToRealNode( node , LEFT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        success = addIntToRealNode( node , RIGHT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none();
    }
    else if(type1 == integerT && type2 == noType)
    {
        error( none ) success = addNullToIntNode( node , RIGHT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == noType && type2 == integerT)
    {
        error( none ) success = addNullToIntNode( node , LEFT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == floatingT && type2 == noType)
    {
        error( none ) success = addNullToRealNode( node , RIGHT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == noType && type2 == floatingT)
    {
        error( none ) success = addNullToRealNode( node , LEFT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == boolT && type2 == noType)
    {
        error( none ) success = addNullToBoolNode( node , RIGHT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == noType && type2 == boolT)
    {
        error( none ) success = addNullToBoolNode( node , LEFT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == stringT && type2 == noType)
    {
        error( none ) success = addNullToStringNode( node , RIGHT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    else if(type1 == noType && type2 == stringT)
    {
        error( none ) success = addNullToStringNode( node , LEFT );
        if( is_error( success ))
        {
            forward_error( success , none);
        }
        return_none() ;
    }
    return_none();
}

error( dType ) checkExpression(PT_Node_ptr node , bool mode)
{
    if( node == NULL )
    {
        return_error( ERROR_SEM_NODE , dType );
    }
    dType type1 = notDefined;
    dType type2 = notDefined;

    //if both ptrs are null that means I found identOfvar ot var
    if(node->leftChild == NULL && node->rightSibling == NULL)
    {
        get_value( dType , tmp , getTerminalDType( node ) , dType);
        return_value( tmp , dType);
    }

    //checking data type of left child of operation
    error( dType ) tmp1 = checkExpression( node->leftChild , mode);
    if( is_error( tmp1 ))
    {
        forward_error( tmp1 , dType);
    }
    type1 = tmp1._value;

    //checking data type of right child of opration
    tmp1 = checkExpression( node->rightSibling , mode);
    if( is_error( tmp1 ))
    {
        forward_error( tmp1 , dType);
    }
    type2 = tmp1._value;

    if(( type1 == boolT || type2 == boolT ) && mode == false)
    {
        return_error( ERROR_SEM_TYPE , dType ); //bool can't be inside expression it can be only as final d type
    }

    tokenType disc = node->data.type.terminal->discriminant;
    if(  disc == concatenation )
    {
        if( (type1 == integerT || type2 == integerT) || (type1 == floatingT || type2 == floatingT) || (type1 == boolT || type2 == boolT))
        {
            return_error( ERROR_SEM_TYPE , dType ); //you can only concatenate two strings
        }
        else if(type1 == stringT || type2 == stringT)
        {
            return_value(stringT, dType);
        }
        else if (type1 == noType || type2 == noType)
        {
            error( none ) success = operandConversion( node , type1 , type2 );
            if( is_error( success ))
            {
                forward_error( success , dType);
            }
            return_value( stringT , dType);
        }
    }
    else if( disc == plusSign || disc == minusSign || disc == multiply || disc == division)
    {
        if( type1 == stringT || type2 == stringT || type1 == stringTNull || type2 == stringTNull )
        {
            return_error( ERROR_SEM_TYPE , dType ); //no conversion for string
        }
        //if any operand is type bool then error shuld occur
        if( type1 == boolT || type2 == boolT )
        {
            return_error( ERROR_SEM_TYPE , dType ); //bool can't be inside expression it can be only as final d type
        }
        //division allways returns float
        if( disc == division)
        {
            error( none ) success = operandConversion( node , type1 , type2 );
            if( is_error( success ))
            {
                forward_error( success , dType);
            }
            return_value( floatingT , dType);
        }
        //if they are same then I just send it forward
        if( type1 == type2 )
        {
            return_value( type1 , dType );
        }
        else if( type1 == floatingT || type2 == floatingT )
        {
            //one of operands is float so conversion must happen
            error( none ) success = operandConversion( node , type1 , type2 );
            if( is_error( success ))
            {
                forward_error( success , dType);
            }
            else
            {
                return_value( floatingT , dType );
            }
        }
        else if(( type1 == integerT || type1 == noType ) && ( type2 == integerT || type2 == noType))
        {
            //one of operands is null so conversion must happen
            error( none ) success = operandConversion( node , type1 , type2 );
            if( is_error( success ))
            {
                forward_error( success , dType);
            }
            else
            {
                return_value( floatingT , dType );
            }
        }
        else
        {
            if( type1 != notDefined )
            {
                return_value(type1 , dType );
            }
            if( type2 != notDefined )
            {
                return_value(type2 , dType );
            }
        }
    }
    else if( disc == lessOper || disc == moreOper || disc == lessOrEqOper || disc == moreOrEqOper)
    {
        if( type1 == type2 )
        {
            return_value( type1 , dType );
        }
        else if( type1 == floatingT || type2 == floatingT )
        {
            //one of operands is float so conversion must happen
            error( none ) success = operandConversion( node , type1 , type2 );
            if( is_error( success ))
            {
                forward_error( success , dType);
            }
            else
            {
                return_value( floatingT , dType );
            }
        }
        else if(( type1 == integerT || type1 == noType ) && ( type2 == integerT || type2 == noType))
        {
            //one of operands is null so conversion must happen
            error( none ) success = operandConversion( node , type1 , type2 );
            if( is_error( success ))
            {
                forward_error( success , dType);
            }
            else
            {
                return_value( floatingT , dType );
            }
        }
    }
    else
    {
        return_value( boolT , dType );
    }
    return_value( boolT , dType );
}

error( dType ) checkExpressionAssigment( PT_Node_ptr node )
{
    if( node == NULL )
    {
        return_error( ERROR_SEM_NODE , dType );
    }
    dType finalType = notDefined;

    error( dType ) success = checkExpression( node , false );

    if( is_error( success ))
    {
        forward_error( success , dType );
    }
    finalType = success._value;
    if( finalType == boolT)
    {
        return_error( ERROR_SEM_TYPE , dType );
    }
    return_value( finalType , dType);
}

error( dType ) checkExpressionIfWhile( PT_Node_ptr node )
{
    if( node == NULL )
    {
        return_error( ERROR_SEM_NODE , dType );
    }
    dType finalType = notDefined;

    PT_Node_ptr terminal = node->leftChild;

    error( dType ) success = checkExpression( terminal , true );

    if( is_error( success ))
    {
        forward_error( success , dType );
    }
    finalType = success._value;

    if( finalType != boolT )
    {
        PT_Node_ptr new = NULL;
        PT_Node_ptr tmp = node->leftChild;
        token_ptr newToken = ( token_ptr) malloc( sizeof( token_t ));
 
        if( newToken == NULL )
        {
            return_error( ERROR_MAL , dType );
        }
        if( finalType == integerT )
        {
            newToken->discriminant = intToBool;
        }
        else if( finalType == floatingT )
        {
            newToken->discriminant = floatToBool;
        }
        else
        {
            newToken->discriminant = stringToBool;
        }
        newToken->info.integer = 0;

        PT_Data_t data = { .isTerminal = true , .type.terminal = newToken };

        error( PT_Node_ptr ) newNodeE = PT_CreateNode( data );
        if( is_error( newNodeE ))
        {
            forward_error( newNodeE , dType );
        }
        new = newNodeE._value;
        new->leftChild = tmp;
        new->rightSibling = NULL;
        node->leftChild = new;
    }
    return_value( boolT , dType );
}