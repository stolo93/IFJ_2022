// Non-terminals branch of to the left
// rules continue to the right

#include <stdio.h>
#include "headers/codegen.h"
#include "headers/IFJ_2022.h"
#include "headers/codegen_context.h"

#define is_worthless_discriminant(discriminant) ((discriminant) == def || (discriminant) == lineComment || (discriminant) == multiLineComm || (discriminant) == openParen || (discriminant) == closeParen || (discriminant) == openSetParen || (discriminant) == closeSetParen || (discriminant) == comma || (discriminant) == semicolon || (discriminant) == colon || (discriminant) == multiLineCommPE)
#define filter(node_variable_name) do { node_variable_name = node_variable_name->rightSibling; } while ((node_variable_name) != NULL && (node_variable_name)->data.isTerminal && is_worthless_discriminant((node_variable_name->data.type.terminal->discriminant)))
#define is_terminal(node, discriminant_enum) (((node)->data.isTerminal) && (node)->data.type.terminal->discriminant == (discriminant_enum))
#define is_non_terminal(node, discriminant_enum) (!((node)->data.isTerminal) && (node)->data.type.nonTerminal == (discriminant_enum))
#define extract_data(node, info_type) ((node)->data.type.terminal->info.info_type)
#define get_discriminant(node) ((node)->data.type.terminal->discriminant)

#define set_node_from_error(node, error_obj) node = (error_obj)._value

// Almost every terminal/non-terminal check in this code will never be hit if the rest of the program has no errors,
// as such most of these checks may be removed if/when everything is properly tested.

void print_normalized_string(const char* string) {
    for (size_t i = 0; string[i] != '\0'; i++) {
        char letter = string[i];
        if (letter <= 32 || letter == 35 || letter == 92) {
            printf("\\%03d", letter);
        } else {
            putchar(letter);
        }
    }
}


void recurse_expression(PT_Node_t* expression_tree) {
    if (expression_tree == NULL) {
        return;
    }

    recurse_expression(expression_tree->leftChild);
    recurse_expression(expression_tree->rightSibling);

    if (get_discriminant(expression_tree) == division) {
        printf("DIVS\n");
    } else if (get_discriminant(expression_tree) == multiply) {
        printf("MULS\n");
    } else if (get_discriminant(expression_tree) == plusSign) {
        printf("ADDS\n");
    } else if (get_discriminant(expression_tree) == minusSign) {
        printf("SUBS\n");
    } else if (get_discriminant(expression_tree) == integer) {
        int value = extract_data(expression_tree, integer);
        printf("PUSHS int@%d\n", value);
    } else if (get_discriminant(expression_tree) == decNum) {
        double value = extract_data(expression_tree, decNuber);
        printf("PUSHS float@%a\n", value);
    } else if (get_discriminant(expression_tree) == string) {
        const char* value = extract_data(expression_tree, string);
        printf("PUSHS string@");
        print_normalized_string(value);
        printf("\n");
    } else if (get_discriminant(expression_tree) == nullT) {
        printf("PUSHS nil@nil\n");
    } else if (get_discriminant(expression_tree) == identOfVar) {
        const char* value = extract_data(expression_tree, string);
        printf("PUSHS LF@%s\n", value);
    } else if (get_discriminant(expression_tree) == concatenation) {
        printf("POPS GF@RBX\nPOPS GF@RAX\nCONCAT GF@RAX GF@RAX GF@RBX\nPUSHS GF@RAX\n");
    } else if (get_discriminant(expression_tree) == EqOper) {
        printf("EQS\n");
    } else if (get_discriminant(expression_tree) == EqOper) {
        printf("EQS\nNOTS\n");
    }  else if (get_discriminant(expression_tree) == lessOper) {
        printf("LTS\n");
    } else if (get_discriminant(expression_tree) == moreOper) {
        printf("GTS\n");
    } else if (get_discriminant(expression_tree) == lessOrEqOper) {
        printf("GTS\nNOTS\n");
    }  else if (get_discriminant(expression_tree) == moreOrEqOper) {
        printf("LTS\nNOTS\n");
    } else if (get_discriminant(expression_tree) == intToReal) {
        printf("INT2FLOATS\n");
    } else if (get_discriminant(expression_tree) == intToBool) {
        printf("PUSHS int@0\nEQS\nNOTS\n");
    } else if (get_discriminant(expression_tree) == floatToBool) {
        printf("PUSHS float@0x0p+0\nEQS\nNOTS\n");
    } else if (get_discriminant(expression_tree) == intToReal) {
        printf("PUSHS string@\nEQS\nNOTS\n");
    }
}

error(none) run_expression(PT_Node_t *expression_start) {
    PT_Node_ptr current = expression_start->leftChild;
    if (current->data.isTerminal == false) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    recurse_expression(current->leftChild);
    recurse_expression(current->rightSibling);

    if (get_discriminant(current) == division) {
        printf("DIVS\n");
    } else if (get_discriminant(current) == multiply) {
        printf("MULS\n");
    } else if (get_discriminant(current) == plusSign) {
        printf("ADDS\n");
    } else if (get_discriminant(current) == minusSign) {
        printf("SUBS\n");
    } else if (get_discriminant(current) == integer) {
        int value = extract_data(current, integer);
        printf("PUSHS int@%d\n", value);
    } else if (get_discriminant(current) == decNum) {
        double value = extract_data(current, decNuber);
        printf("PUSHS float@%a\n", value);
    } else if (get_discriminant(current) == string) {
        const char* value = extract_data(current, string);
        printf("PUSHS string@");
        print_normalized_string(value);
        printf("\n");
    } else if (get_discriminant(current) == nullT) {
        printf("PUSHS nil@nil\n");
    } else if (get_discriminant(current) == identOfVar) {
        const char* value = extract_data(current, string);
        printf("PUSHS LF@%s\n", value);
    } else if (get_discriminant(current) == concatenation) {
        printf("POPS GF@RBX\nPOPS GF@RAX\nCONCAT GF@RAX GF@RAX GF@RBX\nPUSHS GF@RAX\n");
    } else if (get_discriminant(current) == EqOper) {
        printf("EQS\n");
    } else if (get_discriminant(current) == EqOper) {
        printf("EQS\nNOTS\n");
    }  else if (get_discriminant(current) == lessOper) {
        printf("LTS\n");
    } else if (get_discriminant(current) == moreOper) {
        printf("GTS\n");
    } else if (get_discriminant(current) == lessOrEqOper) {
        printf("GTS\nNOTS\n");
    }  else if (get_discriminant(current) == moreOrEqOper) {
        printf("LTS\nNOTS\n");
    } else if (get_discriminant(current) == intToReal) {
        printf("INT2FLOATS\n");
    } else if (get_discriminant(current) == intToBool) {
        printf("PUSHS int@0\nEQS\nNOTS\n");
    } else if (get_discriminant(current) == floatToBool) {
        printf("PUSHS float@0x0p+0\nEQS\nNOTS\n");
    } else if (get_discriminant(current) == intToReal) {
        printf("PUSHS string@\nEQS\nNOTS\n");
    }

    return_none();
}

error(PT_Node_ptr) print_while(PT_Node_t* while_start, int* label_indexer, const char* label_prefix, vec_context* context);
error(PT_Node_ptr) call_function(PT_Node_t *call_start);

error(none) print_body(PT_Node_t* body_node, int* label_indexer, const char* label_prefix ,vec_context* context) {
    if (is_non_terminal(body_node, STATEMENT) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }


    while (body_node != NULL) {
        PT_Node_ptr statement = body_node->leftChild;
        body_node = body_node->rightSibling->leftChild;

        if (statement->data.isTerminal == false) {
            // Skip non-terminals
            continue;
        }

        token_t* terminal = statement->data.type.terminal;

        switch (terminal->discriminant) {
            case ifT: {
                error(PT_Node_ptr) continued = print_while(statement, label_indexer, label_prefix, context);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }
                break;
            }
            case returnT: {
                filter(statement);
                if (is_non_terminal(statement, RET_VAL) == false) {
                    vec_context_destroy(context);
                    return_error(ERROR_GEN_UNSPECIFIED, none);
                }

                if (statement->leftChild == NULL) {
                    filter(statement);
                    printf("RETURN\n");
                } else {
                    if (is_non_terminal(statement->leftChild, EXPR) == false) {
                        vec_context_destroy(context);
                        return_error(ERROR_GEN_UNSPECIFIED, none);
                    }

                    error(none) result = run_expression(statement->leftChild);
                    if (is_error(result)) {
                        vec_context_destroy(context);
                        forward_error(result, none);
                    }

                    // Expression results are kept on stack, so we need to save them in a temp that we can return;
                    printf("RETURN\n");
                }

                break;
            }

            case whileT: {
                error(PT_Node_ptr) continued = print_while(statement, label_indexer, label_prefix, context);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }
                break;
            }
            case identOfFunct: {
                error(PT_Node_ptr) continued = call_function(statement);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }

                // If the function returns a value it will be on the stack, we aren't saving the value, so we clear the stack
                printf("CLEARS/n");
                break;
            }

            case identOfVar: {
                const char* variable_name = extract_data(statement, string);
                if (vec_context_find_sorted(context, (char_ptr)variable_name) == false) {
                    printf("DEFVAR LF@%s\n", variable_name);
                    vec_context_insert_sorted(context, (char_ptr)variable_name);
                }

                statement = statement->rightSibling;
                if (is_terminal(statement, assigment) == false) {
                    vec_context_destroy(context);
                    return_error(ERROR_GEN_UNSPECIFIED, none);
                }
                filter(statement);

                if (is_non_terminal(statement, EXPR)) {
                    error(none) result = run_expression(statement);
                    if (is_error(result)) {
                        vec_context_destroy(context);
                        forward_error(result, none);
                    }

                } else if (is_terminal(statement, identOfFunct)) {
                    error(PT_Node_ptr) continued = call_function(statement);
                    if (is_error(continued)) {
                        vec_context_destroy(context);
                        forward_error(continued, none);
                    }
                }

                printf("POPS LF@%s\n", variable_name);
                break;
            }
            default:
                // No other terminal type should be encountered here
                return_error(ERROR_GEN_UNSPECIFIED, none);
        }
    }

    return_none();
}

error(PT_Node_ptr) print_if(PT_Node_t* if_start, int* label_indexer, const char* label_prefix, vec_context* context) {
    if_start = if_start->rightSibling;
    error(none) result = run_expression(if_start);
    if (is_error(result)) {
        forward_error(result, PT_Node_ptr);
    }

    int index_if_block = *label_indexer;
    int index_else_block = *label_indexer + 1;
    int index_end_block = *label_indexer + 2;
    *label_indexer = *label_indexer + 3;

    // Figure out the resulting type of the expression
    printf("POPS GF@RAX\nPUSH GF@RAX\nTYPE GF@RAX GF@RAX\n");
    // Use the proper comparison function
    printf("JUMPIFEQ .%s.%d GF@RAX \"string\"\n", label_prefix, *label_indexer);
    printf("JUMPIFEQ .%s.%d GF@RAX \"int\"\n", label_prefix, *label_indexer + 1);
    printf("JUMPIFEQ .%s.%d GF@RAX \"float\"\n", label_prefix, *label_indexer + 2);
    printf("JUMPIFEQ .%s.%d GF@RAX \"bool\"\n", label_prefix, *label_indexer + 3);

    // String comparison function
    // Falls thorught to the int comparison function since we're comparing the length of the string which is an int
    printf("LABEL .%s.%d\nSTRLEN GF@RAX GF@RAX\n", label_prefix, *label_indexer);

    // int comparison function
    printf("LABEL .%s.%d\nJUMPIFNEQ .%s.%d GF@RAX int@0\nJUMP .%s.%d\n", label_prefix, *label_indexer + 1, label_prefix, index_if_block, label_prefix, index_else_block);

    // float comparison function
    printf("LABEL .%s.%d\nJUMPIFNEQ .%s.%d GF@RAX float@0x0p+0\nJUMP .%s.%d\n", label_prefix, *label_indexer + 2, label_prefix, index_if_block, label_prefix, index_else_block);

    // bool comparison function
    printf("LABEL .%s.%d\nJUMPIFNEQ .%s.%d GF@RAX float@0x0p+0\nJUMP .%s.%d\n", label_prefix, *label_indexer + 2, label_prefix, index_if_block, label_prefix, index_else_block);
    *label_indexer = *label_indexer + 4;

    // if block
    printf("LABEL .%s.%d\n", label_prefix, index_if_block);

    filter(if_start);
    if (if_start->leftChild != NULL) {
        print_body(if_start->leftChild, label_indexer, label_prefix ,context);
    }

    printf("JUMP .%s.%d\n", label_prefix, index_end_block);

    // else block
    printf("LABEL .%s.%d\n", label_prefix, index_else_block);
    filter(if_start);
    if (is_terminal(if_start, elseT) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }
    filter(if_start);
    if (if_start->leftChild != NULL) {
        print_body(if_start->leftChild, label_indexer, label_prefix, context);
    }
    filter(if_start);

    // exit
    printf("LABEL .%s.%d\n", label_prefix, index_end_block);

    return_value(if_start, PT_Node_ptr);
}

error(PT_Node_ptr) print_while(PT_Node_t* while_start, int* label_indexer, const char* label_prefix, vec_context* context) {
    while_start = while_start->rightSibling;
    filter(while_start);

    error(none) result = run_expression(while_start);
    if (is_error(result)) {
        forward_error(result, PT_Node_ptr);
    }

    int index_while_block = *label_indexer;
    int index_end_block = *label_indexer + 1;
    *label_indexer = *label_indexer + 2;

    // Figure out the resulting type of the expression
    printf("POPS GF@RAX\nPUSH GF@RAX\nTYPE GF@RAX GF@RAX\n");
    // Use the proper comparison function
    printf("JUMPIFEQ .%s.%d GF@RAX \"string\"\n", label_prefix, *label_indexer);
    printf("JUMPIFEQ .%s.%d GF@RAX \"int\"\n", label_prefix, *label_indexer + 1);
    printf("JUMPIFEQ .%s.%d GF@RAX \"float\"\n", label_prefix, *label_indexer + 2);
    printf("JUMPIFEQ .%s.%d GF@RAX \"bool\"\n", label_prefix, *label_indexer + 3);

    // String comparison function
    // Falls thorught to the int comparison function since we're comparing the length of the string which is an int
    printf("LABEL .%s.%d\nSTRLEN GF@RAX GF@RAX\n", label_prefix, *label_indexer);

    // int comparison function
    printf("LABEL .%s.%d\nJUMPIFNEQ .%s.%d GF@RAX int@0\nJUMP .%s.%d\n", label_prefix, *label_indexer + 1, label_prefix, index_while_block, label_prefix, index_end_block);

    // float comparison function
    printf("LABEL .%s.%d\nJUMPIFNEQ .%s.%d GF@RAX float@0x0p+0\nJUMP .%s.%d\n", label_prefix, *label_indexer + 2, label_prefix, index_while_block, label_prefix, index_end_block);

    // bool comparison function
    printf("LABEL .%s.%d\nJUMPIFNEQ .%s.%d GF@RAX float@0x0p+0\nJUMP .%s.%d\n", label_prefix, *label_indexer + 2, label_prefix, index_while_block, label_prefix, index_end_block);
    *label_indexer = *label_indexer + 4;

    // while block
    printf("LABEL .%s.%d\n", label_prefix, index_while_block);
    filter(while_start);
    print_body(while_start->leftChild, label_indexer, label_prefix ,context);
    printf("LABEL .%s.%d\n", label_prefix, index_end_block);

    filter(while_start);
    return_value(while_start, PT_Node_ptr);
}

error(none) print_call_args(PT_Node_t * args) {
    if (is_non_terminal(args, ARG_TYPE) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    while (args != NULL) {
        if (is_non_terminal(args->leftChild, TERM)) {
            if (get_discriminant(args->leftChild->leftChild) == nullT) {
                printf("PUSHS nill@nill\n");
            } else if (get_discriminant(args->leftChild->leftChild) == decNum) {
                double value = extract_data(args->leftChild->leftChild, decNuber);
                printf("PUSHS float@%a\n", value);
            } else if (get_discriminant(args->leftChild->leftChild) == integer) {
                int value = extract_data(args->leftChild->leftChild, integer);
                printf("PUSHS int@%d\n", value);
            } else if (get_discriminant(args->leftChild->leftChild) == string) {
                const char* value = extract_data(args->leftChild->leftChild, string);
                printf("PUSHS float@%s\n", value);
            }
        } else {
            if (is_terminal(args->leftChild, identOfVar) == false) {
                return_error(ERROR_GEN_UNSPECIFIED, none);
            }
            printf("PUSHS LF@%s\n", extract_data(args->leftChild, string));
        }

        args = args->rightSibling->leftChild;
    }

    return_none();
}

error(none) call_write_on_args(PT_Node_t* write_args_start) {
    if (write_args_start == NULL) {
        return_none();
    }

    if (is_non_terminal(write_args_start->leftChild, TERM)) {
        PT_Node_t* data = write_args_start->leftChild->leftChild;
        switch (get_discriminant(data)) {
            case nullT:
                printf("WRITE nil@nil\n");
                break;
            case integer:
                printf("WRITE int@%d\n", extract_data(data, integer));
                break;
            case decNum:
                printf("WRITE float@%a\n", extract_data(data, decNuber));
                break;
            case string:
                printf("WRITE string@");
                print_normalized_string(extract_data(data, string));
                printf("\n");
                break;
            default:
                return_error(ERROR_GEN_UNSPECIFIED, none);

        }
    } else if (is_terminal(write_args_start->leftChild, identOfVar)) {
        printf("WRITE LF@%s\n", extract_data(write_args_start->leftChild, string));
    }

    return call_write_on_args(write_args_start->rightSibling->leftChild);
}

error(PT_Node_ptr) call_function(PT_Node_t *call_start) {
    // Setting up and clearing a stack is the purpose of the calling function

    if (is_terminal(call_start, identOfFunct) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }

    const char* function_name = extract_data(call_start, string);

    if (strcmp(function_name, "reads") == 0) {
        printf("READ GF@RAX string\nPUSHS GF@RAX\n");
        filter(call_start);
        if (is_non_terminal(call_start, ARGS) == false) {
            return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
        }
        filter(call_start);

        return_value(call_start, PT_Node_ptr);

    } else if (strcmp(function_name, "readi") == 0) {
        printf("READ GF@RAX int\nPUSHS GF@RAX\n");
        filter(call_start);
        if (is_non_terminal(call_start, ARGS)) {
            return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
        }
        return_value(call_start, PT_Node_ptr);

    } else if (strcmp(function_name, "readf") == 0) {
        printf("READ GF@RAX float\nPUSHS GF@RAX\n");
        filter(call_start);
        if (is_non_terminal(call_start, ARGS)) {
            return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
        }
        return_value(call_start, PT_Node_ptr);

    } else if (strcmp(function_name, "write") == 0) {
        filter(call_start);
        if (call_start->leftChild != NULL) {
            call_write_on_args(call_start->leftChild);
        }

        return_value(call_start, PT_Node_ptr);
    }

    printf("CREATEFRAME\nPUSHFRAME\n");
    if (is_non_terminal(call_start, ARGS) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }

    if (call_start->leftChild == NULL) {
        filter(call_start);
        return_value(call_start, PT_Node_ptr);
    }

    print_call_args(call_start->leftChild);

    printf("POPFRAME\n");
    filter(call_start);
    return_value(call_start, PT_Node_ptr);
}

error(none) load_function_args(PT_Node_t* args, vec_context* context) {
    filter(args);
    if (args == NULL) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    if (is_non_terminal(args, TYPE)) {
        args = args->rightSibling;
    }
    filter(args);

    if (is_terminal(args, identOfVar) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    if (args->rightSibling == NULL) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    if (args->rightSibling->leftChild != NULL) {
        load_function_args(args->rightSibling->leftChild, context);
    }

    const char* var_name = extract_data(args, string);

    vec_context_insert_sorted(context, (char_ptr)var_name);
    printf("DEFVAR %s\nPOPS %s\n", var_name, var_name);
    return_none();
}

error(PT_Node_ptr) define_function(PT_Node_t *function_node, int* label_indexer) {
    PT_Node_ptr current = function_node->rightSibling;
    if (current == NULL)  {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }

    // Defining a function without an identifier is not allowed
    if (is_terminal(current, identOfFunct) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }

    const char* function_name = extract_data(current, string);

    // context keeps track of which variable have already been defined
    vec_context context = new_vec_context();

    // Get access to <BODY> by ignoring <TYPE> all the other trash
    filter(current);

    printf("JUMP %s.end\n", function_name);
    printf("LABEL %s\n", function_name);

    filter(current);
    if (is_non_terminal(current, ARGS) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }

    if (current->leftChild != NULL) {
        error(none) result = load_function_args(current->leftChild, &context);
        if (is_error(result)) {
            forward_error(result, PT_Node_ptr);
        }
    }

    // Find <BODY> node
    filter(current);
    if (is_non_terminal(current, TYPE)) {
        current = current->rightSibling;
    }
    filter(current);

    if (is_non_terminal(current, BODY) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, PT_Node_ptr);
    }

    if (current->leftChild != NULL) {
        print_body(current->leftChild, label_indexer, function_name, &context);
    }
    filter(current);

    printf("LABEL %s.end\n", function_name);
    vec_context_destroy(&context);
    return_value(current, PT_Node_ptr);
}

error(none) generate_code_from_syntax_tree(tree_t* tree) {
    PT_Node_t* current = *tree;
    // Check for prolog and skip it if we get it

    if (is_non_terminal(current, PROLOG)) {
        current = current->leftChild;
    }

    if (is_terminal(current, prolog)) {
        current = current->rightSibling;
    }

    vec_context context = new_vec_context();
    int label_indexer = 0;

    // Define a base frame we use instead of the global to simplify defining variables
    printf("CREATEFRAME\nPUSHFRAME\n");

    // Define helper variables
    printf("DEFVAR GF@RAX\nDEFVAR GF@RBX\n");

    // Main printer loop
    while (current != NULL) {
        // We should always get a <PROG> non-terminal here
        if(is_non_terminal(current, PROG) == false) {
            vec_context_destroy(&context);
            return_error(ERROR_GEN_UNSPECIFIED, none);
        } 
        
        if (current->leftChild == NULL) {
            vec_context_destroy(&context);
            // A non-terminal should not be at the end of the program
            return_error(ERROR_GEN_UNSPECIFIED, none);
        }

        current = current->leftChild;
        PT_Data_t data = current->data;
        // A <PROG> succeeded by a non-terminal should be succeeded by an Expression
        if (data.isTerminal == false) {
            if (data.type.nonTerminal != EXPR) {
                vec_context_destroy(&context);
                return_error(ERROR_GEN_UNSPECIFIED, none);
            }

            // Expressions are in essence pure functions and as such if they're output is not saved executing them has
            // no effect se we skip them
            current = current->rightSibling;
            continue;
        }

        token_t* terminal = data.type.terminal;

        switch (terminal->discriminant) {
            case functionT: {
                error(PT_Node_ptr) continued = define_function(current, &label_indexer);
                if (is_error(continued)) {
                    vec_context_destroy(&context);
                    forward_error(continued, none);
                }
                set_node_from_error(current, continued);
                break;
            }
            case ifT: {
                error(PT_Node_ptr) continued = print_if(current, &label_indexer, "", &context);
                if (is_error(continued)) {
                    vec_context_destroy(&context);
                    forward_error(continued, none);
                }
                set_node_from_error(current, continued);
                break;
            }
            case returnT: {
                filter(current);
                if (is_non_terminal(current, RET_VAL) == false) {
                    vec_context_destroy(&context);
                    return_error(ERROR_GEN_UNSPECIFIED, none);
                }

                if (current->leftChild == NULL) {
                    filter(current);
                    printf("EXIT int@0\n");
                } else {
                    if (is_non_terminal(current->leftChild, EXPR) == false) {
                        vec_context_destroy(&context);
                        return_error(ERROR_GEN_UNSPECIFIED, none);
                    }

                    error(none) result = run_expression(current->leftChild);
                    if (is_error(result)) {
                        vec_context_destroy(&context);
                        forward_error(result, none);
                    }

                    // Expression results are kept on stack, so we need to save them in a temp that we can return;
                    printf("POPS GF@RAX\nEXIT GF@RAX\n");
                    filter(current);
                }

                break;
            }

            case whileT: {
                error(PT_Node_ptr) continued = print_while(current, &label_indexer, "", &context);
                if (is_error(continued)) {
                    vec_context_destroy(&context);
                    forward_error(continued, none);
                }
                set_node_from_error(current, continued);
                break;
            }
            case identOfFunct: {
                error(PT_Node_ptr) continued = call_function(current);
                if (is_error(continued)) {
                    vec_context_destroy(&context);
                    forward_error(continued, none);
                }
                set_node_from_error(current, continued);

                // If the function returns a value it will be on the stack, we aren't saving the value, so we clear the stack
                printf("CLEARS/n");
                break;
            }

            case identOfVar: {

                const char* variable_name = extract_data(current, string);
                if (vec_context_find_sorted(&context, (char_ptr)variable_name) == false) {
                    printf("DEFVAR LF@%s\n", variable_name);
                    vec_context_insert_sorted(&context, (char_ptr)variable_name);
                }

                current = current->rightSibling;
                if (is_terminal(current, assigment) == false) {
                    vec_context_destroy(&context);
                    return_error(ERROR_GEN_UNSPECIFIED, none);
                }
                filter(current);

                if (is_non_terminal(current, EXPR)) {
                    error(none) result = run_expression(current);
                    if (is_error(result)) {
                        vec_context_destroy(&context);
                        forward_error(result, none);
                    }

                } else if (is_terminal(current, identOfFunct)) {
                    error(PT_Node_ptr) continued = call_function(current);
                    if (is_error(continued)) {
                        vec_context_destroy(&context);
                        forward_error(continued, none);
                    }
                    set_node_from_error(current, continued);
                }

                printf("POPS LF@%s\n", variable_name);
                break;
            }
            case endOfFile:
                goto end;
            default:
                // No other terminal type should be encountered here
                return_error(ERROR_GEN_UNSPECIFIED, none);
        }
    }

    end:
    vec_context_destroy(&context);
    // Cleanup the last stack frame and exit with default value
    printf("POPFRAME/nEXIT int@0\n");
    return_none();
}