/**
 * @file prog_tree.c
 * @author Samuel Stolarik (xstola03@stud.fit.vutbr.cz)
 * @brief Implementation of tree for storing program contents
 * @date 2022-10-21
 *
 */

#include "./headers/IFJ_2022.h"
#include "./headers/syntax_tree.h"


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
