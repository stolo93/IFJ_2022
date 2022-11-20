/**
 * @file prog_tree.c
 * @author Samuel Stolarik (xstola03@stud.fit.vutbr.cz)
 * @brief Implementation of tree for storing program contents
 * @date 2022-10-21
 *
 */

#include "./headers/IFJ_2022.h"
#include "./headers/syntax_tree.h"
#include "./headers/syntactic_analysis.h"
#include <stdio.h>

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(PT_Node_ptr, pnode);

error( PT_Node_ptr )  PT_CreateNode ( PT_Data_t data )
{
    PT_Node_t * new_node = malloc( sizeof( PT_Node_t ) );
    if ( new_node == NULL )
    {
        return_error(ERROR_MAL, PT_Node_ptr);
    }

    new_node->data = data;
    new_node->leftChild = NULL;
    new_node->rightSibling = NULL;

    return_value(new_node, PT_Node_ptr);
}


error( PT_Node_ptr ) PT_AddSibling ( PT_Node_t * node, PT_Data_t data )
{
    if ( node == NULL )
    {
        return_error( INVALID_VAL, PT_Node_ptr );
    }

    while( node->rightSibling )
    {
        node = node->rightSibling;
    }

    error(PT_Node_ptr) tmp_node = PT_CreateNode ( data );
    get_value(PT_Node_ptr, new_node, tmp_node, PT_Node_ptr);

    node->rightSibling = new_node;

    return_value( node->rightSibling, PT_Node_ptr );
}


error( PT_Node_ptr ) PT_AddChild ( PT_Node_t * node, PT_Data_t data )
{
    if ( node == NULL )
    {
        return_error( INVALID_VAL, PT_Node_ptr );
    }

    if ( node->leftChild == NULL ) //In case @p node was childless
    {
        error(PT_Node_ptr) tmp_node = PT_CreateNode( data );
        get_value(PT_Node_ptr, new_node, tmp_node, PT_Node_ptr);

        node->leftChild = new_node;

        return_value( node->leftChild, PT_Node_ptr );
    }

    else
    {
        error( PT_Node_ptr ) result = PT_AddSibling( node->leftChild, data );
        get_value(PT_Node_ptr, new_node, result, PT_Node_ptr);

        return_value( new_node, PT_Node_ptr );
    }
}

error( PT_Node_ptr ) PT_FromPostFix( vec_token_ptr * token_stack )
{
    if ( token_stack == NULL )
    {
        return_error(INVALID_VAL, PT_Node_ptr );
    }

    vec_pnode aux_vector = new_vec_pnode();
    error(PT_Node_ptr) tmp_node;
    error(token_ptr) tmp_token;
    PT_Data_t tmp_data;
    PT_Node_t * cur_node = NULL;

    static const tokenType operators[] = {plusSign, minusSign, multiply, division, concatenation, lessOper, lessOrEqOper, moreOper, moreOrEqOper, EqOper, notEqOper, N_VLD };

    while ( vec_token_ptr_len(token_stack) != 0 )
    {
        tmp_token = vec_token_ptr_pop_front(token_stack);
        if ( is_error(tmp_token) ) { vec_pnode_destroy(&aux_vector); forward_error(tmp_token, PT_Node_ptr);}

        tmp_data = (PT_Data_t) {.isTerminal = true, .type.terminal = tmp_token._value};
        tmp_node = PT_CreateNode(tmp_data);
        if ( is_error(tmp_node) ) { vec_pnode_destroy(&aux_vector); forward_error(tmp_node, PT_Node_ptr);}
        cur_node = tmp_node._value;

        if ( isInTokens(cur_node->data.type.terminal->discriminant, operators) )
        {
            // Insert first child of the operand
            tmp_node = vec_pnode_pop_front(&aux_vector);
            if ( is_error(tmp_node) ) { vec_pnode_destroy(&aux_vector); forward_error(tmp_node, PT_Node_ptr);}

            cur_node->leftChild = tmp_node._value;

            // Insert the second child of the operand
            tmp_node = vec_pnode_pop_front(&aux_vector);
            if ( is_error(tmp_node) ) { vec_pnode_destroy(&aux_vector); forward_error(tmp_node, PT_Node_ptr);}

            cur_node->rightSibling = tmp_node._value;
        }

        vec_pnode_push_back(&aux_vector, cur_node);

    }

    vec_pnode_destroy(&aux_vector);
    return_value( cur_node, PT_Node_ptr );
}


void PT_PrintTerminals ( PT_Node_t * node )
{
    if ( node == NULL )
    {
        return;
    }

    if ( node->data.isTerminal )
    {
        PT_PrintTokenType(node->data.type.terminal->discriminant);
    }

    PT_PrintTerminals( node->leftChild );
    PT_PrintTerminals ( node->rightSibling );

}

error( none ) PT_DeleteNode ( PT_Node_t  ** node )
{
    if ( node == NULL )
    {
        return_error( INVALID_VAL, none );
    }

    if ( (*node) == NULL)
    {
        return_none();
    }

    PT_DeleteNode( &(*node)->leftChild );
    PT_DeleteNode( &(*node)->rightSibling );

    // In case the node contains token, free it
    if ( (*node)->data.isTerminal )
    {
        free((*node)->data.type.terminal);
    }

    free(*node);

    *node = NULL;

    return_none();
}

void PT_PrintTokenType ( tokenType discriminant )
{
    char * name;
    switch ( discriminant )
    {
    case elseT:
        name = "else";
        break;
    case functionT:
        name = "function";
        break;
    case ifT:
        name = "if";
        break;
    case nullT:
        name = "NULL";
        break;
    case returnT:
        name = "return";
        break;
    case voidT:
        name = "void";
        break;
    case whileT:
        name = "while";
        break;
    case identOfFunct:
        name = "F_ID";
        break;
    case identOfVar:
        name = "V_ID";
        break;
    case identOfType:
        name = "TYPE";
        break;
    case integer:
        name = "int";
        break;
    case decNum:
        name = "float";
        break;
    case string:
        name = "string";
        break;
    case plusSign:
        name = "+";
        break;
    case minusSign:
        name = "-";
        break;
    case multiply:
        name = "*";
        break;
    case division:
        name = "/";
        break;
    case concatenation:
        name = ".";
        break;
    case lessOper:
        name = "<";
        break;
    case lessOrEqOper:
        name = "<=";
        break;
    case moreOper:
        name = ">";
        break;
    case moreOrEqOper:
        name = ">=";
        break;
    case EqOper:
        name = "===";
        break;
    case notEqOper:
        name = "!=";
        break;
    case assigment:
        name = "=";
        break;
    case openParen:
        name = "(";
        break;
    case closeParen:
        name = ")";
        break;
    case openSetParen:
        name = "{";
        break;
    case closeSetParen:
        name = "}";
        break;
    case comma:
        name = ",";
        break;
    case semicolon:
        name = ";";
        break;
    case colon:
        name = ":";
        break;
    case endOfFile:
        name = "EOF";
        break;
    case identOfTypeN:
        name = "TYPE_N";
        break;
    case prolog:
        name = "prolog";
        break;
    default:
        return;
        break;
    }

    printf("%s ", name);
    return;
}
