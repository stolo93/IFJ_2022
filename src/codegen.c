/****************************************************************
 * @name codegen.c
 * @author : Adam Bezák <xbezak02@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/

#include <stdio.h>
#include "headers/codegen.h"
#include "headers/IFJ_2022.h"
#include "headers/codegen_context.h"

#define is_worthless_discriminant(discriminant) ((discriminant) == def || (discriminant) == lineComment || (discriminant) == multiLineComm || (discriminant) == openParen || (discriminant) == closeParen || (discriminant) == openSetParen || (discriminant) == closeSetParen || (discriminant) == comma || (discriminant) == semicolon || (discriminant) == colon || (discriminant) == multiLineCommPE)
#define is_terminal(node, discriminant_enum) (((node)->data.isTerminal) && (node)->data.type.terminal->discriminant == (discriminant_enum))
#define is_non_terminal(node, discriminant_enum) (!((node)->data.isTerminal) && (node)->data.type.nonTerminal == (discriminant_enum))
#define extract_data(node, info_type) ((node)->data.type.terminal->info.info_type)
#define get_discriminant(node) ((node)->data.type.terminal->discriminant)

#define skip_to_next_nonTerminal(node_ptr, nonTerminal) while (is_non_terminal((node_ptr), (nonTerminal)) == false) { node_ptr = (node_ptr)->rightSibling;}



void print_normalized_string(const char* string) {
    size_t i = 0;
    if (string[i] == '\"') {
        i++;
    }

    for (; string[i] != '\0' && string[i] != '\"'; i++) {
        char letter = string[i];
        if (letter == '\\') {
            i++;
            switch (string[i]) {
                case 'a':
                    printf("\\%03d", '\a');
                    break;
                case 'b':
                    printf("\\%03d", '\b');
                    break;
                case 'f':
                    printf("\\%03d", '\f');
                    break;
                case 'n':
                    printf("\\%03d", '\n');
                    break;
                case 'r':
                    printf("\\%03d", '\r');
                    break;
                case 't':
                    printf("\\%03d", '\t');
                    break;
                case 'v':
                    printf("\\%03d", '\v');
                    break;
                case '\\':
                    printf("\\%03d", '\\');
                    break;
                case '\'':
                    printf("\\%03d", '\'');
                    break;
                case '\"':
                    printf("\\%03d", '\"');
                    break;
                case '?':
                    printf("\\%03d", '\?');
                    break;
                case '0':
                    printf("\\%03d", '\0');
                    break;
                default:
                    // This should never happen
                    break;

            }
        } else if (letter <= 32 || letter == '#') {
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

error(none) print_while(PT_Node_t* while_start, int* label_indexer, const char* label_prefix, vec_context* context);
error(none) call_function(PT_Node_t *call_start, int* labeler);


error(none) print_body_without_new_context(PT_Node_t* body_node, int* label_indexer, const char* label_prefix ,vec_context* context) {
    if (is_non_terminal(body_node, STATEMENT) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    while (body_node != NULL) {
        PT_Node_ptr statement = body_node->leftChild;

        // Set body_node to the next statement token
        body_node = body_node->rightSibling->leftChild;

        if (statement->data.isTerminal == false) {
            // Skip non-terminals
            continue;
        }

        token_t* terminal = statement->data.type.terminal;

        switch (terminal->discriminant) {
            case ifT: {
                error(none) error = print_while(statement, label_indexer, label_prefix, context);
                if (is_error(error)) {
                    vec_context_destroy(context);
                    forward_error(error, none);
                }
                break;
            }
            case returnT: {
                skip_to_next_nonTerminal(statement, RET_VAL);

                if (statement->leftChild != NULL) {
                    error(none) result = run_expression(statement->leftChild);
                    if (is_error(result)) {
                        vec_context_destroy(context);
                        forward_error(result, none);
                    }
                }

                printf("RETURN\n");
                break;
            }

            case whileT: {
                error(none) continued = print_while(statement, label_indexer, label_prefix, context);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }
                break;
            }
            case identOfFunct: {
                error(none) continued = call_function(statement, label_indexer);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }

                // If the function returns a value it will be on the stack, we aren't saving the value, so we clear the stack
                printf("CLEARS\n");
                break;
            }

            case identOfVar: {
                const char* variable_name = extract_data(statement, string);
                if (vec_context_find_sorted(context, (char_ptr)variable_name) == false) {
                    printf("DEFVAR LF@%s\n", variable_name);
                    vec_context_insert_sorted(context, (char_ptr)variable_name);
                }

                skip_to_next_nonTerminal(statement, RVAL);

                if (is_non_terminal(statement->leftChild, EXPR)) {
                    run_expression(statement->leftChild);

                } else if (is_terminal(statement->leftChild, identOfFunct)) {
                    call_function(statement->leftChild, label_indexer);
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

error(none) print_body(PT_Node_t* body_node, int* label_indexer, const char* label_prefix ,vec_context* outer_context) {
    get_value(vec_context, context , clone_context_vec(outer_context), none);

    error(none) result = print_body_without_new_context(body_node, label_indexer, label_prefix, &context);
    if (is_error(result)) {
        vec_context_destroy(&context);
    }

    return result;
}


error(none) print_if(PT_Node_t* if_start, int* label_indexer, const char* label_prefix, vec_context* context) {
    skip_to_next_nonTerminal(if_start, EXPR);
    error(none) result = run_expression(if_start);
    if (is_error(result)) {
        forward_error(result, none);
    }

    int index_if_block = *label_indexer;
    int index_else_block = *label_indexer + 1;
    int index_end_block = *label_indexer + 2;
    *label_indexer = *label_indexer + 3;

    // Figure out the resulting type of the expression
    printf("POPS GF@RAX\nMOVE GF@RBX GF@RAX\nPUSHS GF@RAX\nTYPE GF@RBX GF@RAX\n");
    // Use the proper comparison function
    printf("JUMPIFEQ -%s-%d GF@RBX string@string\n", label_prefix, *label_indexer);
    printf("JUMPIFEQ -%s-%d GF@RBX string@int\n", label_prefix, *label_indexer + 1);
    printf("JUMPIFEQ -%s-%d GF@RBX string@float\n", label_prefix, *label_indexer + 2);
    printf("JUMPIFEQ -%s-%d GF@RBX string@bool\n", label_prefix, *label_indexer + 3);
    printf("JUMPIFEQ -%s-%d GF@RBX string@nil\n", label_prefix, index_else_block);

    // String comparison function
    // Falls thorught to the int comparison function since we're comparing the length of the string which is an int
    printf("LABEL -%s-%d\nSTRLEN GF@RAX GF@RAX\n", label_prefix, *label_indexer);

    // int comparison function
    printf("LABEL -%s-%d\nJUMPIFNEQ -%s-%d GF@RAX int@0\nJUMP -%s-%d\n", label_prefix, *label_indexer + 1, label_prefix, index_if_block, label_prefix, index_else_block);

    // float comparison function
    printf("LABEL -%s-%d\nJUMPIFNEQ -%s-%d GF@RAX float@0x0p+0\nJUMP -%s-%d\n", label_prefix, *label_indexer + 2, label_prefix, index_if_block, label_prefix, index_else_block);

    // bool comparison function
    printf("LABEL -%s-%d\nJUMPIFEQ -%s-%d GF@RAX bool@false\n", label_prefix, *label_indexer + 3, label_prefix, index_else_block);
    *label_indexer = *label_indexer + 4;

    // if block
    printf("LABEL -%s-%d\n", label_prefix, index_if_block);

    skip_to_next_nonTerminal(if_start, BODY);
    if (if_start->leftChild != NULL) {
        print_body_without_new_context(if_start->leftChild, label_indexer, label_prefix ,context);
    }

    printf("JUMP -%s-%d\n", label_prefix, index_end_block);

    // else block
    printf("LABEL -%s-%d\n", label_prefix, index_else_block);

    if_start = if_start->rightSibling; // skip_to_next... skip_to_next would just return the current body token if we didn't move to the right
    skip_to_next_nonTerminal(if_start, BODY);

    if (if_start->leftChild != NULL) {
        print_body_without_new_context(if_start->leftChild, label_indexer, label_prefix, context);
    }

    // exit
    printf("LABEL -%s-%d\n", label_prefix, index_end_block);

    return_none();
}

error(none) print_while(PT_Node_t* while_start, int* label_indexer, const char* label_prefix, vec_context* context) {
    skip_to_next_nonTerminal(while_start, EXPR)

    int index_while_if_block = *label_indexer;
    int index_while_block = *label_indexer + 1;
    int index_end_block = *label_indexer + 2;
    *label_indexer = *label_indexer + 3;

    printf("LABEL -%s-%d\n", label_prefix, index_while_if_block);
    error(none) result = run_expression(while_start);
    if (is_error(result)) {
        forward_error(result, none);
    }



    // Figure out the resulting type of the expression
    printf("POPS GF@RAX\nMOVE GF@RBX GF@RAX\nPUSHS GF@RAX\nTYPE GF@RAX GF@RAX\n");
    // Use the proper comparison function
    printf("JUMPIFEQ -%s-%d GF@RBX string@string\n", label_prefix, *label_indexer);
    printf("JUMPIFEQ -%s-%d GF@RBX string@int\n", label_prefix, *label_indexer + 1);
    printf("JUMPIFEQ -%s-%d GF@RBX string@float\n", label_prefix, *label_indexer + 2);
    printf("JUMPIFEQ -%s-%d GF@RBX string@bool\n", label_prefix, *label_indexer + 3);
    printf("JUMPIFEQ -%s-%d GF@RBX string@nil\n", label_prefix, index_end_block);

    // String comparison function
    // Falls through to the int comparison function since we're comparing the length of the string which is an int
    printf("LABEL -%s-%d\nSTRLEN GF@RAX GF@RAX\n", label_prefix, *label_indexer);

    // int comparison function
    printf("LABEL -%s-%d\nJUMPIFNEQ -%s-%d GF@RAX int@0\nJUMP -%s-%d\n", label_prefix, *label_indexer + 1, label_prefix, index_while_block, label_prefix, index_end_block);

    // float comparison function
    printf("LABEL -%s-%d\nJUMPIFNEQ -%s-%d GF@RAX float@0x0p+0\nJUMP -%s-%d\n", label_prefix, *label_indexer + 2, label_prefix, index_while_block, label_prefix, index_end_block);

    // bool comparison function
    // We can just fall through this part instead of jumping to the while block
    printf("LABEL -%s-%d\nJUMPIFEQ -%s-%d GF@RAX float@0x0p+0\n", label_prefix, *label_indexer + 2, label_prefix, index_end_block);
    *label_indexer = *label_indexer + 4;

    // while block
    printf("LABEL -%s-%d\n", label_prefix, index_while_block);
    skip_to_next_nonTerminal(while_start, BODY);
    print_body_without_new_context(while_start->leftChild, label_indexer, label_prefix ,context);
    printf("LABEL -%s-%d\n", label_prefix, index_end_block);


    return_none();
}

error(none) print_call_args(PT_Node_t * args) {
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

        skip_to_next_nonTerminal(args, ARGS_NEXT);
        args = args->leftChild;
    }

    return_none();
}

error(none) call_write_on_args(PT_Node_t* write_args_start) {
    if (write_args_start == NULL) {
        return_none();
    }

    skip_to_next_nonTerminal(write_args_start, ARG_TYPE);

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

error(none) call_function(PT_Node_t *call_start, int* labeler) {
    // Setting up and clearing a stack is the job of the caller
    const char* function_name = extract_data(call_start, string);
    skip_to_next_nonTerminal(call_start, ARGS);

    if (strcmp(function_name, "reads") == 0) {
        printf("READ GF@RAX string\nPUSHS GF@RAX\n");
        return_none();
    } else if (strcmp(function_name, "readi") == 0) {
        printf("READ GF@RAX int\nPUSHS GF@RAX\n");
        return_none();
    } else if (strcmp(function_name, "readf") == 0) {
        printf("READ GF@RAX float\nPUSHS GF@RAX\n");
        return_none();
    } else if (strcmp(function_name, "write") == 0) {
        if (call_start->leftChild != NULL) {
            call_write_on_args(call_start->leftChild);
        }
        return_none();
    } else if (strcmp(function_name, "floatval") == 0) {
        run_expression(call_start->rightSibling->rightSibling);
        printf("POPS GF@RBX\nTYPE GF@RAX GF@RBX\nPUSHS GF@RBX\n");
        printf("JUMPIFEQ -%d GF@RAX float\n", *labeler);
        printf("JUMPIFEQ -%d GF@RAX int\n", (*labeler) + 1);
        printf("JUMPIFEQ -%d GF@RAX nil\n", (*labeler) + 2);
        printf("EXIT 53\n");
        printf("LABEL -%d\nint2floats\nJUMP -%d", (*labeler) + 1, (*labeler));
        printf("LABEL -%d\nPOPS GF@RBX\nPUSHS float@0x0p+0\n", (*labeler) + 2);
        printf("LABEL -%d\n", (*labeler));
        *labeler = (*labeler) + 3;

        return_none();
    } else if (strcmp(function_name, "intval") == 0) {
        run_expression(call_start->rightSibling->rightSibling);
        printf("POPS GF@RBX\nTYPE GF@RAX GF@RBX\nPUSHS GF@RBX\n");
        printf("JUMPIFEQ -%d GF@RAX int\n", *labeler);
        printf("JUMPIFEQ -%d GF@RAX float\n", (*labeler) + 1);
        printf("JUMPIFEQ -%d GF@RAX nil\n", (*labeler) + 2);
        printf("EXIT 53\n");
        printf("LABEL -%d\nfloat2ints\nJUMP -%d", (*labeler) + 1, (*labeler));
        printf("LABEL -%d\nPOPS GF@RBX\nPUSHS int@0\n", (*labeler) + 2);
        printf("LABEL -%d\n", (*labeler));
        *labeler = (*labeler) + 3;

        return_none();
    } else if (strcmp(function_name, "strval") == 0) {
        run_expression(call_start->rightSibling->rightSibling);
        printf("POPS GF@RBX\nTYPE GF@RAX GF@RBX\nPUSHS GF@RBX\n");
        printf("JUMPIFEQ -%d GF@RAX string\n", *labeler);
        printf("JUMPIFEQ -%d GF@RAX nil\n", (*labeler) + 1);
        printf("EXIT 53\n");
        printf("LABEL -%d\nPOPS GF@RBX\nPUSHS string@\n", (*labeler) + 1);
        printf("LABEL -%d\n", (*labeler));
        *labeler = (*labeler) + 2;

        return_none();
    } else if (strcmp(function_name, "strlen") == 0) {
        run_expression(call_start->rightSibling->rightSibling);
        printf("POPS GF@RAX\nSTRLEN GF@RAX GF@RAX\nPUSHS GF@RAX\n");
    } else if (strcmp(function_name, "substring") == 0) {

    } else if (strcmp(function_name, "ord") == 0) {

    } else if (strcmp(function_name, "chr") == 0) {

    }

    // If we don't push arguments we're done
    if (call_start->leftChild == NULL) {
        return_none();
    }

    print_call_args(call_start->leftChild);

    printf("POPFRAME\n");
    return_none();
}

error(none) load_function_args(PT_Node_t* args, vec_context* context) {
    if (args == NULL) {
        return_none();
    }

    args = args->rightSibling;
    const char* var_name = extract_data(args, string);
    skip_to_next_nonTerminal(args, PARAMS_NEXT);

    load_function_args(args->leftChild, context);

    vec_context_insert_sorted(context, (char_ptr)var_name);
    printf("DEFVAR %s\nPOPS %s\n", var_name, var_name);
    return_none();
}

error(none) define_function(PT_Node_t *function_node, int* label_indexer) {
    PT_Node_ptr current = function_node->rightSibling;
    const char* function_name = extract_data(current, string);

    // context keeps track of which variable have already been defined
    vec_context context = new_vec_context();

    printf("JUMP %s.end\n", function_name);
    printf("LABEL %s\n", function_name);

    // Skip to function parameters
    skip_to_next_nonTerminal(current, PARAMS);
    load_function_args(current->leftChild, &context);

    skip_to_next_nonTerminal(current, BODY);

    if (current->leftChild != NULL) {
        print_body(current->leftChild, label_indexer, function_name, &context);
    }

    printf("LABEL %s.end\n", function_name);
    vec_context_destroy(&context);
    return_none();
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
    printf(".IFJcode22\nCREATEFRAME\nPUSHFRAME\n");

    // Define helper variables
    printf("DEFVAR GF@RAX\nDEFVAR GF@RBX\n");

    // Main printer loop
    while (current != NULL) {

        current = current->leftChild;
        PT_Data_t data = current->data;
        // A <PROG> succeeded by a non-terminal should be succeeded by an Expression
        if (data.isTerminal == false) {
            if (data.type.nonTerminal != EXPR) {
                vec_context_destroy(&context);
                return_error(ERROR_GEN_UNSPECIFIED, none);
            }

            // Expressions are in essence pure functions and as such if their output is not saved therefore executing them
            // has no effect se we skip them
            current = current->rightSibling;
            continue;
        }

        token_t* terminal = data.type.terminal;

        switch (terminal->discriminant) {
            case functionT: {
                define_function(current, &label_indexer);
                break;
            }
            case ifT: {
                print_if(current, &label_indexer, "", &context);
                break;
            }
            case returnT: {
                skip_to_next_nonTerminal(current, RET_VAL);

                if (current->leftChild == NULL) {
                    printf("EXIT int@0\n");
                } else {
                    run_expression(current->leftChild);
                    // Expression results are kept on stack, so we need to save them in rax so we have a variable to return
                    printf("POPS GF@RAX\nEXIT GF@RAX\n");

                }
                break;
            }

            case whileT: {
                print_while(current, &label_indexer, "", &context);
                break;
            }
            case identOfFunct: {
                call_function(current, &label_indexer);
                // If the function returns a value it will be on the stack, we aren't saving the value, so we clear the stack
                printf("CLEARS\n");
                break;
            }

            case identOfVar: {

                const char* variable_name = extract_data(current, string);
                if (vec_context_find_sorted(&context, (char_ptr)variable_name) == false) {
                    printf("DEFVAR LF@%s\n", variable_name);
                    vec_context_insert_sorted(&context, (char_ptr)variable_name);
                }

                skip_to_next_nonTerminal(current, RVAL);

                if (is_non_terminal(current->leftChild, EXPR)) {
                    run_expression(current->leftChild);

                } else if (is_terminal(current->leftChild, identOfFunct)) {
                    call_function(current->leftChild, &label_indexer);
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

        skip_to_next_nonTerminal(current, PROG);
    }

    end:
    vec_context_destroy(&context);
    // Cleanup the last stack frame and exit with default value
    printf("POPFRAME\nEXIT int@0");
    return_none();
}