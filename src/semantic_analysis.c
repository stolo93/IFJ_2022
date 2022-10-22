/**
 * @file semantic_analysis.c
 * @author Vojtěch Novotný (xnovot1t@stud.fit.vutbr.cz)
 *
 * @brief Implementation of semantic analysis for IFJ22 language
 *
 * @date 2022-10-22
 *
 */

#include "./headers/semantic_analysis.h"

vec_htab_t_ptr symtable_vector;

//Checks:
/* Undefined function - OK */
/* Redefined function - OK */
/* Invalid function call argument count - OK */
/* Invalid function call argument type - OK */
/* Invalid function return value type - OK */
/* Usage of undefined variable - OK */
/* Invalid return value expression count */
/* Type compatibility in expression error */
/* Other semantic errors */

error(_Bool) sendProgTree(PT_Node_ptr root)
{
    if(root == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    //create vector of symtables
    symtable_vector = new_vec_htab_t_ptr();
    error(htab_t_ptr) symtableErrorObj = htab_init(DEFAULT_SYMTABLE_SIZE);
    get_value(htab_t_ptr, symtable, symtableErrorObj, _Bool);
    vec_htab_t_ptr_push_front(&symtable_vector, symtable);

    error(_Bool) errorObj = getAllFunctionDefinitions(root, symtable);
    if(is_error(errorObj))
    {
        vec_htab_t_ptr_destroy(&symtable_vector);
        forward_error(errorObj, _Bool);
    }

    errorObj = goThroughProgTreeNodes(root);
    if(is_error(errorObj))
    {
        vec_htab_t_ptr_destroy(&symtable_vector);
        forward_error(errorObj, _Bool);
    }

    vec_htab_t_ptr_destroy(&symtable_vector);
    return_value(true, bool);
}

error(_Bool) getAllFunctionDefinitions(PT_Node_ptr root, htab_t_ptr table)
{
    error(PT_Node_ptr) errorObj;
    if (root == NULL || table == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    errorObj = findProgNode(root);  //save first progNode into tmpNode
    get_value(PT_Node_ptr, progNode, errorObj, _Bool);
    if(progNode == NULL)
    {
        return_value(true, _Bool);
    }

    PT_Node_ptr tmpNode = progNode;

    while(tmpNode->leftChild)
    {
        tmpNode = tmpNode->leftChild;
        if(tmpNode->data.type.terminal->discriminant == functionT)
        {
            error(htab_pair_t_ptr) errorObj;
            errorObj = htab_find(table, tmpNode->rightSibling->data.type.terminal->info.string);
            get_value(htab_pair_t_ptr, functionDefinitionRecord, errorObj, _Bool);
            if(functionDefinitionRecord != NULL)
            {
                return_error(ERROR_SEM_REDEF_FUNC, _Bool);
            }
            else
            {
                error(htab_pair_t_ptr) htabErrorObj =  htab_lookup_add(table, tmpNode->rightSibling->data.type.terminal->info.string, function);
                get_value(htab_pair_t_ptr, functionRecord, htabErrorObj, _Bool);

                error(htab_t_ptr) symtableErrorObj = htab_init(DEFAULT_SYMTABLE_SIZE);
                get_value(htab_t_ptr, symtable, symtableErrorObj, _Bool);
                vec_htab_t_ptr_push_front(&symtable_vector, symtable);

                error(_Bool) fdefErrorObj = checkFunctionDefinition(tmpNode, functionRecord);

                vec_htab_t_ptr_pop_front(&symtable_vector);
                if(is_error(fdefErrorObj))
                {
                    forward_error(fdefErrorObj, _Bool);
                }
            }
        }

        errorObj = findProgNode(tmpNode);   //get next progNode into tmpNode
        get_value(PT_Node_ptr, nextProgNode, errorObj, _Bool);
        if(nextProgNode == NULL)
        {
            return_value(true, _Bool);
        }
        tmpNode = nextProgNode;
    }
    return_value(true, bool);
}

error(_Bool) goThroughProgTreeNodes(PT_Node_ptr node)
{
    if (node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    error(PT_Node_ptr) errorObj = findProgNode(node);  //save first progNode into tmpNode
    get_value(PT_Node_ptr, progNode, errorObj, _Bool);
    if(progNode == NULL)
    {
        return_value(true, _Bool);
    }

    PT_Node_ptr tmpNode = progNode;

    while(tmpNode->leftChild)
    {
        error(_Bool) checkNodeErrorObj = checkProgTreeNode(tmpNode, NULL);
        if(is_error(checkNodeErrorObj))
        {
            forward_error(checkNodeErrorObj, _Bool);
        }

        error(PT_Node_ptr) progNodeErrorObj = findProgNode(tmpNode);   //get next progNode into tmpNode
        get_value(PT_Node_ptr, nextProgNode, progNodeErrorObj, _Bool);
        if(nextProgNode == NULL)
        {
            return_value(true, _Bool);
        }
        tmpNode = nextProgNode;
    }
    return_value(true, bool);
}

error(_Bool) checkProgTreeNode(PT_Node_ptr node, htab_pair_t_ptr functionContext)
{
    error(_Bool) errorObj;
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    if(node->data.isTerminal == false)
    {
        return_value(true, bool);
    }

    switch (node->data.type.terminal->discriminant)
    {
        case identOfFunct:
            errorObj = checkFunctionCall(node);
            if(is_error(errorObj))
            {
                forward_error(errorObj, _Bool);
            }
            break;

        case identOfVar:
            errorObj = checkVariable(node);
            if(is_error(errorObj))
            {
                forward_error(errorObj, _Bool);
            }
            break;

        case ifT:
            errorObj = checkIfBlock(node, functionContext);
            if(is_error(errorObj))
            {
                forward_error(errorObj, _Bool);
            }
            break;

        case whileT:
            errorObj = checkWhileBlock(node, functionContext);
            if(is_error(errorObj))
            {
                forward_error(errorObj, _Bool);
            }
            break;

        case returnT:
            if(functionContext == NULL)
            {
                break;
            }
            errorObj = checkReturnSatement(node, functionContext);
            break;

        default:
            break;
    }

    return_value(true, bool);
}

error(_Bool) checkFunctionDefinition(PT_Node_ptr node, htab_pair_t_ptr functionRecord)
{
    if(node == NULL || functionRecord == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    //get to start of statement list
    PT_Node_ptr currentNode = node;
    while(currentNode->rightSibling)
    {

        if(currentNode->data.type.nonTerminal == BODY)   //found start of statement list
        {
            break;
        }
        else if(currentNode->data.type.nonTerminal == TYPE) //get function return value
        {
            functionRecord->diff.func.outType = tokenTypeToDataType(currentNode->leftChild->leftChild->data.type.terminal->discriminant);
        }
        else if(currentNode->data.type.nonTerminal == PARAMS)   //add function parameters
        {
            if(currentNode->leftChild == NULL)  //function has no parameters
            {
                currentNode = currentNode->rightSibling;
                continue;
            }
            else
            {
                PT_Node_ptr currentParamNode = currentNode->leftChild;
                do
                {
                    structFuncParam param = { .inIdent = (char*) currentParamNode->rightSibling->data.type.terminal->info.string, .inType = tokenTypeToDataType(currentParamNode->leftChild->data.type.terminal->discriminant) };
                    error(none) success = vec_structFuncParam_push_back( &(functionRecord->diff.func.inParams) , param );
                    if(is_error(success))
                    {
                        forward_error(success, _Bool);
                    }

                    currentParamNode = currentParamNode->rightSibling->rightSibling;
                } while(currentParamNode->leftChild);
            }
        }
        else
        {
            currentNode = currentNode->rightSibling;
        }
    }

    if(currentNode->leftChild == NULL)  //function has empty body
    {
        return_value(true, bool);
    }

    currentNode = currentNode->leftChild;
    error(_Bool) errorObj;

    do  //go through statement list
    {
        switch (currentNode->data.type.terminal->discriminant)
        {
            case ifT:
                {
                    error(htab_t_ptr) symtableErrorObj = htab_init(DEFAULT_SYMTABLE_SIZE);
                    get_value(htab_t_ptr, ifSymtable, symtableErrorObj, _Bool);
                    vec_htab_t_ptr_push_front(&symtable_vector, ifSymtable);

                    errorObj = checkIfBlock(currentNode, functionRecord);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    vec_htab_t_ptr_pop_front(&symtable_vector);
                    break;
                }

            case whileT:
                {
                    error(htab_t_ptr) symtableErrorObj = htab_init(DEFAULT_SYMTABLE_SIZE);
                    get_value(htab_t_ptr, whileSymtable, symtableErrorObj, _Bool);
                    vec_htab_t_ptr_push_front(&symtable_vector, whileSymtable);

                    errorObj = checkWhileBlock(currentNode, functionRecord);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    vec_htab_t_ptr_pop_front(&symtable_vector);
                    break;
                }

            case identOfFunct:
                {
                    errorObj = checkFunctionCall(currentNode);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case identOfVar:
                {
                    errorObj = checkVariable(currentNode);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case returnT:
                {
                    errorObj = checkReturnSatement(currentNode, functionRecord);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            default:
                break;
        }

        error(PT_Node_ptr) lastNodeErrorObj = findLastNodeOnRow(currentNode);
        get_value(PT_Node_ptr, lastNodeOnRow, lastNodeErrorObj, _Bool);
        if(lastNodeOnRow == NULL)
        {
            return_value(true, _Bool);
        }
        currentNode = lastNodeOnRow;

    } while(currentNode->leftChild != NULL);

    return_value(true, bool);
}

error(_Bool) checkFunctionCall(PT_Node_ptr node)
{
    if(vec_htab_t_ptr_len(&symtable_vector) == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, _Bool);
    }

    //get symbol table
    error(htab_t_ptr_ptr) symtableErrorObj = vec_htab_t_ptr_get(&symtable_vector, vec_htab_t_ptr_len(&symtable_vector));
    get_value(htab_t_ptr_ptr, symTable, symtableErrorObj, _Bool);

    //get record with function data from symbol table
    error(htab_pair_t_ptr) errorObj = htab_find(*symTable, node->data.type.terminal->info.string); 
    get_value(htab_pair_t_ptr, functionDefinitionRecord, errorObj, _Bool);

    if(functionDefinitionRecord == NULL)
    {
        return_error(ERROR_SEM_UNDEF_FUNC, _Bool);
    }

    PT_Node_ptr argNode = node->rightSibling->rightSibling->leftChild;    //get to first node containing argument

    //function parameters count == 0
    int inParamsVectorLength = vec_structFuncParam_len(&(functionDefinitionRecord->diff.func.inParams));
    if(inParamsVectorLength == 0)
    {
        if(argNode == NULL) //no arguments in progTree
        {
            return_value(true, bool);
        }
        else if(argNode != NULL) //wrong argument count
        {
            return_error(ERROR_SEM_FUNC_ARGCNT_CNT, _Bool);
        }
    }

    //function parameters count > 0
    size_t vectorIndex = 0;
    error(structFuncParam_ptr) vectorErrorObj;

    do{
        if(argNode->leftChild->data.type.nonTerminal == TERM)
        {
            vectorErrorObj = vec_structFuncParam_get(&(functionDefinitionRecord->diff.func.inParams), vectorIndex);
            get_value(structFuncParam_ptr, vectorItem, vectorErrorObj, _Bool);
            if(vectorItem->inType != tokenTypeToDataType(argNode->leftChild->leftChild->data.type.terminal->discriminant))
            {
                return_error(ERROR_SEM_FUNC_ARGCNT_TYPE, _Bool);
            }
        }

        else if(argNode->leftChild->data.type.terminal->discriminant == identOfVar)
        {
            error(htab_pair_t_ptr) errorObj = checkAllSymtableLevels(&(argNode->leftChild->leftChild->data.type.terminal->info.string), variable);
            get_value(htab_pair_t_ptr, variableRecord, errorObj, _Bool);

            if(variableRecord == NULL)
            {
                return_error(ERROR_SEM_UNDEF_VAR, _Bool);
            }

            vectorErrorObj = vec_structFuncParam_get(&(variableRecord->diff.func.inParams), vectorIndex);
            get_value(structFuncParam_ptr, vectorItem, vectorErrorObj, _Bool);

            if(vectorItem->inType != variableRecord->diff.var.dataType)
            {
                return_error(ERROR_SEM_FUNC_ARGCNT_TYPE, _Bool);
            }
        }

        vectorIndex++;
        argNode = argNode->rightSibling;

        //parameters count in progTree > parameters count in vector
        if(vectorIndex == (size_t) (inParamsVectorLength + 1) && argNode->rightSibling != NULL)
        {
            return_error(ERROR_SEM_FUNC_ARGCNT_CNT, _Bool);
        }
    }
    while(argNode->rightSibling != NULL);

    return_value(true, _Bool);
}

error(_Bool) checkVariable(PT_Node_ptr node)
{
    if(vec_htab_t_ptr_len(&symtable_vector) == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, _Bool);
    }

    error(htab_pair_t_ptr) errorObj = checkAllSymtableLevels(&(node->data.type.terminal->info.string), variable);
    get_value(htab_pair_t_ptr, variableRecord, errorObj, _Bool);

    if(variableRecord == NULL)
    {
        return_error(ERROR_SEM_UNDEF_VAR, _Bool);
    }

    if(node->rightSibling->data.type.terminal->discriminant == EqOper)
    {
        PT_Node_ptr rvalNode = node->rightSibling->rightSibling;

        if(rvalNode->leftChild->data.isTerminal && rvalNode->leftChild->data.type.terminal->discriminant == identOfFunct)   //function call
        {
            error(_Bool) functionCallErrorObj = checkFunctionCall(rvalNode->leftChild);
            if(is_error(functionCallErrorObj))
            {
                forward_error(functionCallErrorObj, _Bool);
            }

            error(htab_t_ptr) symtableErrorObj = getMainSymTable();
            get_value(htab_t_ptr, table, symtableErrorObj, _Bool);

            error(htab_pair_t_ptr) errorObj = htab_find(table, rvalNode->leftChild->data.type.terminal->info.string);
            get_value(htab_pair_t_ptr, functionRecord, errorObj, _Bool);

            if(variableRecord->diff.var.dataType != functionRecord->diff.func.outType)
            {
                variableRecord->diff.var.dataType = functionRecord->diff.func.outType;
            }
            return_value(true, bool);
        }

        else if(rvalNode->leftChild->data.isTerminal == false && rvalNode->leftChild->data.type.nonTerminal == TERM) //TERM
        {
            if(variableRecord->diff.var.dataType != tokenTypeToDataType(rvalNode->leftChild->data.type.terminal->discriminant))
            {
                variableRecord->diff.var.dataType = tokenTypeToDataType(rvalNode->leftChild->data.type.terminal->discriminant);
            }
            return_value(true, bool);
        }

        else if(rvalNode->leftChild->data.isTerminal == false && rvalNode->leftChild->data.type.nonTerminal == EXPR) //EXPRESSION
        {
            error(_Bool) errorObj = checkExpression(node);
            get_value(bool, checkExpressionReturnValue, errorObj, _Bool);
            return_value(checkExpressionReturnValue, bool);
        }
    }

    return_value(false, bool);
}

error(_Bool) checkIfBlock(PT_Node_ptr node, htab_pair_t_ptr functionContext)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    PT_Node_ptr checkNode = node->rightSibling->rightSibling;   //if -> ( -> <ARG_TYPE>
    //TODO call checkExpression on checkNode

    checkNode = checkNode->rightSibling->rightSibling->rightSibling;    //if -> ( -> <ARG_TYPE> -> ) -> { -> <BODY>
    error(_Bool) errorObj = checkBodyNonTerminal(checkNode, functionContext);
    if(is_error(errorObj))
    {
        forward_error(errorObj, _Bool);
    }

    checkNode = checkNode->rightSibling->rightSibling->rightSibling->rightSibling;  //if -> ( -> <ARG_TYPE> -> ) -> { -> <BODY> -> } -> else -> { -> <BODY>
    errorObj = checkBodyNonTerminal(checkNode, functionContext);
    if(is_error(errorObj))
    {
        forward_error(errorObj, _Bool);
    }

    return_value(true, bool);
}

error(_Bool) checkWhileBlock(PT_Node_ptr node, htab_pair_t_ptr functionContext)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    PT_Node_ptr checkNode = node->rightSibling->rightSibling;   //while -> ( -> <ARG_TYPE>
    //TODO call checkExpression on checkNode

    checkNode = checkNode->rightSibling->rightSibling->rightSibling;    //while -> ( -> <ARG_TYPE> -> ) -> { -> <BODY>
    error(_Bool) errorObj = checkBodyNonTerminal(checkNode, functionContext);
    if(is_error(errorObj))
    {
        forward_error(errorObj, _Bool);
    }

    return_value(true, bool);
}

error(_Bool) checkExpression(PT_Node_ptr node)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    //TODO

    return_value(false, _Bool);
}

error(_Bool) checkReturnSatement(PT_Node_ptr node, htab_pair_t_ptr functionRecord)
{
    if(node == NULL ||functionRecord == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    if(functionRecord->diff.func.outType != noType && node->rightSibling == NULL)
    {
        return_error(ERROR_SEM_RETVAL_EXPR, _Bool);
    }

    if(node->rightSibling->leftChild->data.isTerminal == true)
    {
        if(node->rightSibling->leftChild->data.type.terminal->discriminant == identOfVar)
        {
            error(_Bool) varErrorObj = checkVariable(node->rightSibling->leftChild);
            if(is_error(varErrorObj))
            {
                forward_error(varErrorObj, _Bool);
            }

            error(htab_pair_t_ptr) recordErrorObj = checkAllSymtableLevels(&(node->rightSibling->leftChild->data.type.terminal->info.string), variable);
            get_value(htab_pair_t_ptr, record, recordErrorObj, _Bool);
            if(functionRecord->diff.func.outType != record->diff.var.dataType)
            {
                return_error(ERROR_SEM_FUNC_RET_TYPE, _Bool);
            }
        }
    }
    else if(node->rightSibling->leftChild->data.isTerminal == false)
    {
        if(node->rightSibling->leftChild->data.type.nonTerminal == TERM)
        {
            if(functionRecord->diff.func.outType != tokenTypeToDataType(node->rightSibling->leftChild->leftChild->data.type.terminal->discriminant))
            {
                return_error(ERROR_SEM_FUNC_RET_TYPE, _Bool);
            }
        }
        else if(node->rightSibling->leftChild->data.type.nonTerminal == EXPR)
        {
            //TODO call checkExpression on node->rightSibling->leftChild
        }
    }

    return_value(true, bool);
}

error(_Bool) checkBodyNonTerminal(PT_Node_ptr node, htab_pair_t_ptr functionContext)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    PT_Node_ptr currentNode = node->leftChild;

    do  //go through statement list
    {
        switch (currentNode->data.type.terminal->discriminant)
        {
            case ifT:
                {

                    error(_Bool) errorObj = checkIfBlock(currentNode, functionContext);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    break;
                }

            case whileT:
                {
                    error(_Bool)  errorObj = checkWhileBlock(currentNode, functionContext);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    break;
                }

            case identOfFunct:
                {
                    error(_Bool)  errorObj = checkFunctionCall(currentNode);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case identOfVar:
                {
                    error(_Bool)  errorObj = checkVariable(currentNode);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case returnT:
                {
                    if(functionContext == NULL)
                    {
                        break;
                    }
                    error(_Bool)  errorObj = checkReturnSatement(currentNode, functionContext);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            default:
                break;
        }

        error(PT_Node_ptr) lastNodeErrorObj = findLastNodeOnRow(currentNode);
        get_value(PT_Node_ptr, lastNodeOnRow, lastNodeErrorObj, _Bool);
        if(lastNodeOnRow == NULL)
        {
            return_value(true, _Bool);
        }
        currentNode = lastNodeOnRow;

    } while(currentNode->leftChild != NULL);

    return_value(true, _Bool);
}

error(htab_t_ptr) getMainSymTable()
{
    if(vec_htab_t_ptr_len(&symtable_vector) == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, htab_t_ptr);
    }

    //get symbol table
    error(htab_t_ptr_ptr) symtableErrorObj = vec_htab_t_ptr_get(&symtable_vector, vec_htab_t_ptr_len(&symtable_vector) - 1);
    get_value(htab_t_ptr_ptr, symTable, symtableErrorObj, htab_t_ptr);

    return_value(*symTable, htab_t_ptr);
}

error(htab_pair_t_ptr) checkAllSymtableLevels(htab_key_t* key, sType type)
{
    if(vec_htab_t_ptr_len(&symtable_vector) == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, htab_pair_t_ptr);
    }

    for(size_t i = 0; i < vec_htab_t_ptr_len(&symtable_vector); i++)
    {
        error(htab_t_ptr_ptr) symtableErrorObj = vec_htab_t_ptr_get(&symtable_vector, i);
        get_value(htab_t_ptr_ptr, table, symtableErrorObj, htab_pair_t_ptr);

        error(htab_pair_t_ptr) errorObj = htab_find(*table, *key);
        get_value(htab_pair_t_ptr, record, errorObj, htab_pair_t_ptr);
        if(record != NULL && record->symType == type)
        {
            return_value(record, htab_pair_t_ptr);
        }
    }

    return_value(NULL, htab_pair_t_ptr);
}

dType tokenTypeToDataType(tokenType type)
{
    switch (type)
    {
        case integer:
            return integerT;
        case decNum:
            return floatingT;
        case string:
            return stringT;
        case voidT:
            return noType;
        default:
            return notDefined;
    }
    return notDefined;
}

error(PT_Node_ptr) findProgNode(PT_Node_t * node)
{
    if (node == NULL)
    {
        return_error(INVALID_VAL, PT_Node_ptr);
    }

    PT_Node_ptr tmpNode = node;

    while(tmpNode->rightSibling)
    {
        tmpNode = tmpNode->rightSibling;
        if(tmpNode->data.type.nonTerminal == PROG)
        {
            return_value(tmpNode, PT_Node_ptr);
        }
    }
    return_value(NULL, PT_Node_ptr);
}

error(PT_Node_ptr) findLastNodeOnRow(PT_Node_t * node)
{
     if (node == NULL)
    {
        return_error(INVALID_VAL, PT_Node_ptr);
    }

    PT_Node_ptr tmpNode = node;

    while(tmpNode != NULL)
    {
        tmpNode = tmpNode->rightSibling;
    }
    return_value(tmpNode, PT_Node_ptr);
}

