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

extern error( dType ) getTerminalDType( PT_Node_ptr node);
extern error( dType ) checkExpression(PT_Node_ptr node , bool mode);
extern error( dType ) checkExpressionAssigment( PT_Node_ptr node );
extern error( dType ) checkExpressionIfWhile( PT_Node_ptr node );

DEFINE_VEC_FUNCTIONS_NO_DESTRUCTOR(htab_t_ptr, htab_t_ptr);
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
        if(tmpNode->data.isTerminal && tmpNode->data.type.terminal->discriminant == functionT)
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

                error(_Bool) fdefErrorObj = checkFunctionDefinition(tmpNode, functionRecord);

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

        error(PT_Node_ptr) progNodeErrorObj = findProgNode(tmpNode->leftChild);   //get next progNode into tmpNode
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
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }
    error(_Bool) errorObj;

    if(node->data.isTerminal == false && node->data.type.nonTerminal != PROG)
    {
        return_value(true, bool);
    }


    if(node->leftChild->data.isTerminal)
    {
        switch (node->leftChild->data.type.terminal->discriminant)
        {
            case functionT:
                {
                    get_value(htab_t_ptr, mainSymtable, getMainSymTable(), _Bool);
                    get_value(htab_pair_t_ptr, functionDefinitionRecord, htab_find(mainSymtable, node->leftChild->rightSibling->data.type.terminal->info.string), _Bool);

                    get_value(htab_t_ptr, funSymtable, htab_init(DEFAULT_SYMTABLE_SIZE), _Bool);
                    vec_htab_t_ptr_push_front(&symtable_vector, funSymtable);

                    errorObj = checkFunctionBody(node->leftChild, functionDefinitionRecord);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    vec_htab_t_ptr_pop_front(&symtable_vector);
                    break;
                }

            case identOfFunct:
                {
                    errorObj = checkFunctionCall(node->leftChild);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case identOfVar:
                {
                    errorObj = checkVariable(node->leftChild);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case ifT:
                {
                    //error(htab_t_ptr) symtableErrorObj = htab_init(DEFAULT_SYMTABLE_SIZE);
                    //get_value(htab_t_ptr, ifSymtable, symtableErrorObj, _Bool);
                    //vec_htab_t_ptr_push_front(&symtable_vector, ifSymtable);

                    errorObj = checkIfBlock(node->leftChild, NULL, functionContext, false);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    //vec_htab_t_ptr_pop_front(&symtable_vector);
                    break;
                }

            case whileT:
                {
                    errorObj = checkWhileBlock(node->leftChild, NULL, functionContext);
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
                    errorObj = checkReturnSatement(node->leftChild, functionContext);
                    break;
                }

            default:
                break;
        }
    }
    else
    {
        switch(node->leftChild->data.type.nonTerminal)
        {
            case EXPR:
                if(node->leftChild->leftChild != NULL)
                {
                    if(node->leftChild->rightSibling->data.type.terminal->discriminant == EqOper)
                    {
                        error(dType) errorObj = checkExpressionAssigment(node->leftChild);
                        if(is_error(errorObj))
                        {
                            forward_error(errorObj, _Bool);
                        }
                    }
                    else
                    {
                        error(dType) errorObj = checkExpression(node->leftChild->leftChild , false);
                        if(is_error(errorObj))
                        {
                            forward_error(errorObj, _Bool);
                        }
                    }
                }
                break;

            default:
                break;
        }
    }

    return_value(true, bool);
}

error(_Bool) checkFunctionBody(PT_Node_ptr node, htab_pair_t_ptr functionRecord)
{
    if(node == NULL || functionRecord == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    int returnStatementCounter = 0;

    //get to start of statement list
    PT_Node_ptr currentNode = node;
    while(currentNode->rightSibling)
    {
        if(currentNode->data.type.nonTerminal == BODY)   //found start of statement list
        {
            break;
        }
        else if(currentNode->data.type.nonTerminal == PARAMS)
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
                    error(dType) errorObj = getNodeType(currentParamNode->leftChild);
                    get_value(dType, type, errorObj, _Bool);
                    error(htab_t_ptr_ptr) symtableErrorObj = vec_htab_t_ptr_get(&symtable_vector, 0);
                    get_value(htab_t_ptr_ptr, symtable, symtableErrorObj, _Bool);

                    get_value(htab_pair_t_ptr, record, htab_find(*symtable, currentParamNode->rightSibling->data.type.terminal->info.string), _Bool);
                    if(record != NULL)
                    {
                        return_error(ERROR_SEM, _Bool);
                    }

                    error(htab_pair_t_ptr) htabErrorObj =  htab_lookup_add(*symtable, currentParamNode->rightSibling->data.type.terminal->info.string, variable);
                    get_value(htab_pair_t_ptr, variableRecord, htabErrorObj, _Bool);

                    variableRecord->diff.var.dataType = type;

                    if(currentParamNode->rightSibling->rightSibling->leftChild == NULL)
                    {
                        break;
                    }

                    currentParamNode = currentParamNode->rightSibling->rightSibling->leftChild->rightSibling;

                } while(currentParamNode->leftChild);
            }
        }
        currentNode = currentNode->rightSibling;
    }

    if(currentNode->leftChild == NULL)  //function has empty body
    {
        if(returnStatementCounter == 0 && functionRecord->diff.func.outType != noType)
        {
            return_error(ERROR_SEM_FUNC_RET_TYPE, _Bool);
        }
        return_value(true, bool);
    }

    currentNode = currentNode->leftChild;
    error(_Bool) errorObj;

    do  //go through statement list
    {
        if(currentNode->leftChild->data.isTerminal == false)    //skip nonTerminal
        {
            get_value(PT_Node_ptr, lastNodeOnRow, findLastNodeOnRow(currentNode), _Bool);
            if(lastNodeOnRow->leftChild == NULL)
            {
                break;
            }
            currentNode = lastNodeOnRow->leftChild;
        }

        switch (currentNode->leftChild->data.type.terminal->discriminant)
        {
            case ifT:
                {
                    errorObj = checkIfBlock(currentNode->leftChild, &returnStatementCounter, functionRecord, false);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case whileT:
                {
                    error(htab_t_ptr) symtableErrorObj = htab_init(DEFAULT_SYMTABLE_SIZE);
                    get_value(htab_t_ptr, whileSymtable, symtableErrorObj, _Bool);
                    vec_htab_t_ptr_push_front(&symtable_vector, whileSymtable);

                    errorObj = checkWhileBlock(currentNode->leftChild, &returnStatementCounter, functionRecord);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }

                    vec_htab_t_ptr_pop_front(&symtable_vector);
                    break;
                }

            case identOfFunct:
                {
                    errorObj = checkFunctionCall(currentNode->leftChild);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case identOfVar:
                {
                    errorObj = checkVariable(currentNode->leftChild);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    break;
                }

            case returnT:
                {
                    errorObj = checkReturnSatement(currentNode->leftChild, functionRecord);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                    returnStatementCounter++;
                    break;
                }

            default:
                break;
        }

        error(PT_Node_ptr) lastNodeErrorObj = findLastNodeOnRow(currentNode);
        get_value(PT_Node_ptr, lastNodeOnRow, lastNodeErrorObj, _Bool);

        if(lastNodeOnRow->leftChild == NULL)
        {
            break;
        }
        currentNode = lastNodeOnRow->leftChild;

    } while(currentNode != NULL);

    if(returnStatementCounter == 0 && functionRecord->diff.func.outType != noType)
    {
        return_error(ERROR_SEM_FUNC_RET_TYPE, _Bool);
    }
    return_value(true, _Bool);
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
        if(currentNode->data.isTerminal == true && currentNode->data.type.terminal->discriminant == colon)
        {
            if(currentNode->rightSibling->data.type.nonTerminal == TYPE)    //get function return value
            {
                error(dType) errorObj = getNodeType(currentNode->rightSibling->leftChild);
                get_value(dType, type, errorObj, _Bool);

                functionRecord->diff.func.outType = type;
            }
            currentNode = currentNode->rightSibling;
            continue;
        }

        if(currentNode->data.type.nonTerminal == BODY)   //found start of statement list
        {
            break;
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
                    error(dType) errorObj = getNodeType(currentParamNode->leftChild);
                    get_value(dType, type, errorObj, _Bool);
                    structFuncParam param = { .inIdent = (char*) currentParamNode->rightSibling->data.type.terminal->info.string, .inType = type};

                    error(none) success = vec_structFuncParam_push_back(&(functionRecord->diff.func.inParams), param);
                    if(is_error(success))
                    {
                        forward_error(success, _Bool);
                    }

                    if(currentParamNode->rightSibling->rightSibling->leftChild == NULL)
                    {
                        break;
                    }

                    currentParamNode = currentParamNode->rightSibling->rightSibling->leftChild->rightSibling;

                } while(currentParamNode->leftChild);
            }
        }

        currentNode = currentNode->rightSibling;
    }
    return_value(true, bool);
}

error(_Bool) checkFunctionCall(PT_Node_ptr node)
{
    if(vec_htab_t_ptr_len(&symtable_vector) == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, _Bool);
    }

    //get symbol table
    get_value(htab_t_ptr, mainSymtable, getMainSymTable(), _Bool);

    //get record with function data from symbol table
    error(htab_pair_t_ptr) errorObj = htab_find(mainSymtable, node->data.type.terminal->info.string);
    get_value(htab_pair_t_ptr, functionDefinitionRecord, errorObj, _Bool);
    if(functionDefinitionRecord == NULL)
    {
        return_error(ERROR_SEM_UNDEF_FUNC, _Bool);
    }

    PT_Node_ptr argNode = node->rightSibling->rightSibling->leftChild;    //get to first node containing argument

    //function parameters count == 0
    int inParamsVectorLength = vec_structFuncParam_len(&(functionDefinitionRecord->diff.func.inParams));
    if(node->rightSibling->rightSibling->leftChild == NULL && inParamsVectorLength > 0)
    {
        return_error(ERROR_SEM_FUNC_ARGCNT_CNT, _Bool);
    }

    if(inParamsVectorLength == 0 || strcmp(functionDefinitionRecord->key , "write") == 0)
    {
        if(node->rightSibling->rightSibling->leftChild == NULL) //no arguments in progTree
        {
            return_value(true, bool);
        }
        else if(argNode != NULL && isSpecialFunction(functionDefinitionRecord->key) == false ) //wrong argument count
        {
            return_error(ERROR_SEM_FUNC_ARGCNT_CNT, _Bool);
        }
    }

    //function parameters count > 0
    size_t vectorIndex = 0;
    error(structFuncParam_ptr) vectorErrorObj;

    while(argNode->rightSibling)
    {
        if(argNode->leftChild->data.type.nonTerminal == TERM)
        {
            if(isSpecialFunction(functionDefinitionRecord->key) == false)
            {
                vectorErrorObj = vec_structFuncParam_get(&(functionDefinitionRecord->diff.func.inParams), vectorIndex);
                get_value(structFuncParam_ptr, vectorItem, vectorErrorObj, _Bool);
                get_value(dType, tmpType, getTerminalDType(argNode->leftChild->leftChild), _Bool);

                if(vectorItem->inType != tmpType && (isTypeNullable(vectorItem->inType) == true && tmpType == noType) == false)
                {
                    return_error(ERROR_SEM_FUNC_ARGCNT_TYPE, _Bool);
                }
            }
        }

        else if(argNode->leftChild->data.type.terminal->discriminant == identOfVar)
        {
            error(htab_pair_t_ptr) errorObj = checkAllSymtableLevels(&(argNode->leftChild->data.type.terminal->info.string));
            get_value(htab_pair_t_ptr, variableRecord, errorObj, _Bool);

            if(variableRecord == NULL)
            {
                return_error(ERROR_SEM_UNDEF_VAR, _Bool);
            }
            if(isSpecialFunction(functionDefinitionRecord->key) == false)
            {
                vectorErrorObj = vec_structFuncParam_get(&(functionDefinitionRecord->diff.func.inParams), vectorIndex);
                get_value(structFuncParam_ptr, vectorItem, vectorErrorObj, _Bool);

                if(vectorItem->inType != variableRecord->diff.var.dataType && checkForTypeCompatibility(vectorItem->inType, variableRecord->diff.var.dataType) == false)
                {
                    return_error(ERROR_SEM_FUNC_ARGCNT_TYPE, _Bool);
                }
            }
        }

        vectorIndex++;

        //parameters count in progTree > parameters count in vector
        if((vectorIndex == (size_t)(inParamsVectorLength) && argNode->rightSibling->leftChild != NULL) && strcmp( functionDefinitionRecord->key , "write") != 0)
        {
            return_error(ERROR_SEM_FUNC_ARGCNT_CNT, _Bool);
        }

        argNode = argNode->rightSibling;
    }

    return_value(true, _Bool);
}

error(_Bool) checkVariable(PT_Node_ptr node)
{
    if(vec_htab_t_ptr_len(&symtable_vector) == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, _Bool);
    }

    htab_pair_t_ptr variableRecord = NULL;
    error(htab_pair_t_ptr) errorObj = checkAllSymtableLevels(&(node->data.type.terminal->info.string));
    get_value(htab_pair_t_ptr, symtableRecord, errorObj, _Bool);
    if(symtableRecord != NULL)
    {
        variableRecord = symtableRecord;
    }
    else if(symtableRecord == NULL)
    {
        error(htab_t_ptr_ptr) errorObj = vec_htab_t_ptr_get(&symtable_vector, 0);
        get_value(htab_t_ptr_ptr, symtable, errorObj, _Bool);

        error(htab_pair_t_ptr) htabErrorObj =  htab_lookup_add(*symtable, node->data.type.terminal->info.string, variable);
        get_value(htab_pair_t_ptr, symtableRecord2, htabErrorObj, _Bool);
        variableRecord = symtableRecord2;
    }

    if(node->rightSibling->data.type.terminal->discriminant == assigment)
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
            error( dType) errorObj = checkExpression(rvalNode->leftChild->leftChild, false);
            get_value(dType, checkExpressionReturnValue, errorObj, _Bool);

            variableRecord->diff.var.dataType = checkExpressionReturnValue;
        }
    }

    return_value(false, bool);
}

error(_Bool) checkIfBlock(PT_Node_ptr node, int* returnStatementCounter, htab_pair_t_ptr functionContext, bool ifContext)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    PT_Node_ptr checkNode = node->rightSibling;   //if -> ( -> <EXPR>
    error(dType) exprResult = checkExpressionIfWhile(checkNode);
    if(is_error(exprResult))
    {
        forward_error(exprResult, _Bool);
    }


    checkNode = checkNode->rightSibling->rightSibling;    //if -> ( -> <EXPR> -> ) -> { -> <BODY>
    error(_Bool) errorObj = checkBodyNonTerminal(checkNode, returnStatementCounter, functionContext, ifContext);
    if(is_error(errorObj))
    {
        forward_error(errorObj, _Bool);
    }

    checkNode = checkNode->rightSibling->rightSibling->rightSibling->rightSibling;  //if -> ( -> <ARG_TYPE> -> ) -> { -> <BODY> -> } -> else -> { -> <BODY>
    errorObj = checkBodyNonTerminal(checkNode, returnStatementCounter, functionContext, false);
    if(is_error(errorObj))
    {
        forward_error(errorObj, _Bool);
    }

    return_value(true, bool);
}

error(_Bool) checkWhileBlock(PT_Node_ptr node, int* returnStatementCounter, htab_pair_t_ptr functionContext)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    PT_Node_ptr checkNode = node->rightSibling;   //while -> ( -> <ARG_TYPE>
    error(dType) exprResult = checkExpressionIfWhile(checkNode);
    if(is_error(exprResult))
    {
        forward_error(exprResult, _Bool);
    }

    checkNode = checkNode->rightSibling->rightSibling;    //while -> ( -> <ARG_TYPE> -> ) -> { -> <BODY>
    error(_Bool) errorObj = checkBodyNonTerminal(checkNode, returnStatementCounter, functionContext, false);
    if(is_error(errorObj))
    {
        forward_error(errorObj, _Bool);
    }

    return_value(true, bool);
}

error(_Bool) checkReturnSatement(PT_Node_ptr node, htab_pair_t_ptr functionRecord)
{
    if(node == NULL ||functionRecord == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    //function return value is not void and return statement is not followed by return value
    if(functionRecord->diff.func.outType != noType)
    {
        if(node->rightSibling == NULL || node->rightSibling->leftChild == NULL)
        {
            return_error(ERROR_SEM_RETVAL_EXPR, _Bool);
        }
    }
    else if(functionRecord->diff.func.outType == noType)
    {
        if(node->rightSibling->leftChild != NULL)
        {
            return_error(ERROR_SEM_RETVAL_EXPR, _Bool);
        }
        else
        {
            return_value(true, _Bool);
        }
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

            error(htab_pair_t_ptr) recordErrorObj = checkAllSymtableLevels(&(node->rightSibling->leftChild->data.type.terminal->info.string));
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
            error(dType) errorObj = checkExpression(node->rightSibling->leftChild->leftChild, false);
            get_value(dType, type, errorObj, _Bool);

            if(node->rightSibling->leftChild->leftChild->data.type.terminal->discriminant == identOfVar)
            {
                error(htab_pair_t_ptr) recordErrorObj = checkAllSymtableLevels(&(node->rightSibling->leftChild->leftChild->data.type.terminal->info.string));
                get_value(htab_pair_t_ptr, record, recordErrorObj, _Bool);
                if(record != NULL)
                {
                    if(functionRecord->diff.func.outType != record->diff.var.dataType && checkForTypeCompatibility(functionRecord->diff.func.outType, record->diff.var.dataType) == false)
                    {
                        return_error(ERROR_SEM_FUNC_RET_TYPE, _Bool);
                    }
                }
                return_value(true, _Bool);
            }
            if(functionRecord->diff.func.outType != type && (isTypeNullable(functionRecord->diff.func.outType) == true && type == noType) == false)
            {
                return_error(ERROR_SEM_FUNC_RET_TYPE, _Bool);
            }
        }
    }

    return_value(true, bool);
}

error(_Bool) checkBodyNonTerminal(PT_Node_ptr node, int* returnStatementCounter, htab_pair_t_ptr functionContext, bool ifContext)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, _Bool);
    }

    if(node->leftChild == NULL) //empty body
    {
        return_value(true, _Bool);
    }

    PT_Node_ptr currentNode = node->leftChild;

    do  //go through statement list
    {
        if(currentNode->data.isTerminal == false)
        {
            if(currentNode->data.type.nonTerminal == STATEMENT && currentNode->leftChild->data.isTerminal == true)
            {
                switch (currentNode->leftChild->data.type.terminal->discriminant)
                {
                    case ifT:
                        {
                            error(_Bool) errorObj = checkIfBlock(currentNode->leftChild, returnStatementCounter, functionContext, ifContext);
                            if(is_error(errorObj))
                            {
                                forward_error(errorObj, _Bool);
                            }
                            break;
                        }

                    case whileT:
                        {
                            error(_Bool)  errorObj = checkWhileBlock(currentNode->leftChild, returnStatementCounter, functionContext);
                            if(is_error(errorObj))
                            {
                                forward_error(errorObj, _Bool);
                            }

                            break;
                        }

                    case identOfFunct:
                        {
                            error(_Bool)  errorObj = checkFunctionCall(currentNode->leftChild);
                            if(is_error(errorObj))
                            {
                                forward_error(errorObj, _Bool);
                            }
                            break;
                        }

                    case identOfVar:
                        {
                            error(_Bool)  errorObj = checkVariable(currentNode->leftChild);
                            if(is_error(errorObj))
                            {
                                forward_error(errorObj, _Bool);
                            }
                            break;
                        }

                    case returnT:
                        {
                            if(functionContext == NULL || ifContext == true)
                            {
                                break;
                            }
                            error(_Bool)  errorObj = checkReturnSatement(currentNode->leftChild, functionContext);
                            if(is_error(errorObj))
                            {
                                forward_error(errorObj, _Bool);
                            }
                            if(returnStatementCounter != NULL)
                            {
                                (*returnStatementCounter)++;
                            }
                            break;
                        }

                    default:
                        break;
                }
            }
            else if(currentNode->data.type.nonTerminal == STATEMENT && currentNode->leftChild->data.isTerminal == false)
            {
                if(currentNode->leftChild->data.type.nonTerminal == EXPR)
                {
                    error( dType) errorObj = checkExpression(currentNode->leftChild->leftChild, false);
                    if(is_error(errorObj))
                    {
                        forward_error(errorObj, _Bool);
                    }
                }
            }
        }

        error(PT_Node_ptr) lastNodeErrorObj = findLastNodeOnRow(currentNode);
        get_value(PT_Node_ptr, lastNodeOnRow, lastNodeErrorObj, _Bool);

        if(lastNodeOnRow->leftChild == NULL)
        {
            return_value(true, _Bool);
        }
        currentNode = lastNodeOnRow->leftChild;

    } while(currentNode != NULL);

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

error(htab_pair_t_ptr) checkAllSymtableLevels(htab_key_t* key)
{
    if(key == NULL)
    {
        return_error(INVALID_VAL, htab_pair_t_ptr);
    }

    size_t vectorSize = vec_htab_t_ptr_len(&symtable_vector);

    if(vectorSize == 0)
    {
        return_error(VECTOR_EMPTY_ERROR, htab_pair_t_ptr);
    }

    for(size_t i = 0; i < vectorSize; i++)
    {
        error(htab_t_ptr_ptr) symtableErrorObj = vec_htab_t_ptr_get(&symtable_vector, i);
        get_value(htab_t_ptr_ptr, table, symtableErrorObj, htab_pair_t_ptr);

        error(htab_pair_t_ptr) errorObj = htab_find(*table, *key);
        get_value(htab_pair_t_ptr, record, errorObj, htab_pair_t_ptr);
        if(record != NULL)
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

error(dType) getNodeType(PT_Node_ptr node)
{
    if(node == NULL)
    {
        return_error(INVALID_VAL, dType);
    }
    if(node->data.type.terminal->discriminant == identOfType)
    {
        if(strcmp(node->data.type.terminal->info.string, "int") == 0)
        {
            return_value(integerT, dType);
        }
        if(strcmp(node->data.type.terminal->info.string, "float") == 0)
        {
            return_value(floatingT, dType);
        }
        if(strcmp(node->data.type.terminal->info.string, "string") == 0)
        {
            return_value(stringT, dType);
        }
        if(strcmp(node->data.type.terminal->info.string, "void") == 0)
        {
            return_value(noType, dType);
        }
    }
    else if (node->data.type.terminal->discriminant == identOfTypeN)
    {
        if(strcmp(node->data.type.terminal->info.string, "int") == 0)
        {
            return_value(integerTNull, dType);
        }
        if(strcmp(node->data.type.terminal->info.string, "float") == 0)
        {
            return_value(floatingTNull, dType);
        }
        if(strcmp(node->data.type.terminal->info.string, "string") == 0)
        {
            return_value(stringTNull, dType);
        }
        if(strcmp(node->data.type.terminal->info.string, "void") == 0)
        {
            return_value(noType, dType);
        }
    }

    return_value(notDefined, dType);
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

    while(tmpNode->rightSibling != NULL)
    {
        tmpNode = tmpNode->rightSibling;
        if(tmpNode->rightSibling == NULL){
            return_value(tmpNode, PT_Node_ptr);
        }
    }

    return_value(tmpNode, PT_Node_ptr);
}

bool isTypeNullable(dType type)
{
    switch (type)
    {
        case integerTNull:
        case floatingTNull:
        case stringTNull:
            return true;
        default:
            return false;
    }
    return false;
}

bool checkForTypeCompatibility(dType type1, dType type2)
{
    if((type1 == integerT || type1 == integerTNull) && (type2 == integerT || type2 == integerTNull))
    {
        return true;
    }
    else if((type1 == floatingT || type1 == floatingTNull) && (type2 == floatingT || type2 == floatingTNull))
    {
        return true;
    }
    else if((type1 == stringT || type1 == stringTNull) && (type2 == stringT || type2 == stringTNull))
    {
        return true;
    }
    else if((type1 == stringT || type1 == noType) && (type2 == stringT || type2 == noType))
    {
        return true;
    }
    return false;
}

bool isSpecialFunction(char* name)
{
    if(name == NULL)
    {
        return false;
    }
    if(strcmp(name , "write") == 0)
    {
        return true;
    }
    else if(strcmp(name , "floatval") == 0)
    {
        return true;
    }
    else if(strcmp(name , "intval") == 0)
    {
        return true;
    }
    else if(strcmp(name , "strval") == 0)
    {
        return true;
    }
    return false;
}