/****************************************************************
 * @name : codegen.c
 * @author : Adam Bezák <xbezak02@stud.fit.vutbr.cz>
 * @brief : A file containing almost everything needed to generate code
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "headers/codegen.h"
#include "headers/IFJ_2022.h"
#include "headers/codegen_context.h"

#define is_worthless_discriminant(discriminant) ((discriminant) == def || (discriminant) == lineComment || (discriminant) == multiLineComm || (discriminant) == openParen || (discriminant) == closeParen || (discriminant) == openSetParen || (discriminant) == closeSetParen || (discriminant) == comma || (discriminant) == semicolon || (discriminant) == colon || (discriminant) == multiLineCommPE)
#define is_terminal(node, discriminant_enum) (((node)->data.isTerminal) && (node)->data.type.terminal->discriminant == (discriminant_enum))
#define is_non_terminal(node, discriminant_enum) (!((node)->data.isTerminal) && (node)->data.type.nonTerminal == (discriminant_enum))
#define extract_data(node, info_type) ((node)->data.type.terminal->info.info_type)
#define get_discriminant(node) ((node)->data.type.terminal->discriminant)

#define skip_to_next_nonTerminal(node_ptr, nonTerminal) while (is_non_terminal((node_ptr), (nonTerminal)) == false) { node_ptr = (node_ptr)->rightSibling;}

size_t print_special_case(const char* substring) {
    if (isdigit(substring[0])) {
        // octals

        if (substring[0] == '8' || substring[0] == '9' || substring[1] == '\0') {
            printf("\\%03d", '\\');
            return 0;
        }

        if (substring[1] < '0' || substring[1] > '7' || substring[2] == '\0') {
            printf("\\%03d", '\\');
            return 0;
        }

        if (substring[2] < '0' || substring[2] > '7') {
            printf("\\%03d", '\\');
            return 0;
        }

        char num[4] = {'\0', '\0', '\0', '\0'};
        num[0] = substring[0];
        num[1] = substring[1];
        num[2] = substring[2];
        long decimal = strtol(num, NULL,8);
        printf("\\%03ld", decimal);
        return 3;

    } else if (substring[0] == 'x') {
        // hexadecimals
        if (isxdigit(substring[1]) == false || isxdigit(substring[2]) == false) {
            printf("\\");
            return 0;
        }

        char num[3] = {'\0', '\0', '\0'};
        num[0] = substring[1];
        num[1] = substring[2];
        long decimal = strtol(num, NULL,16);
        printf("\\%03ld", decimal);
        return 3;
    }
    printf("\\%03d", '\\');
    return 0;
}

void print_normalized_string(const char* string) {
    size_t i = 0;
    if (string[i] == '\"') {
        i++;
    }

    for (; string[i] != '\0' && string[i] != '\"'; i++) {
        char letter = string[i];
        if (letter == '\\') {

            switch (string[i + 1]) {
                case 'n':
                    printf("\\%03d", '\n');
                    i++;
                    break;
                case 'r':
                    printf("\\%03d", '\r');
                    i++;
                    break;
                case 't':
                    printf("\\%03d", '\t');
                    i++;
                    break;
                case 'v':
                    printf("\\%03d", '\v');
                    i++;
                    break;
                case 'e':
                    printf("\\%03d", 0x1B);
                    i++;
                    break;
                case 'f':
                    printf("\\%03d", '\f');
                    i++;
                    break;
                case '\\':
                    printf("\\%03d", '\\');
                    i++;
                    break;
                case '\"':
                    printf("\\%03d", '\"');
                    i++;
                    break;
                case '$':
                    putchar('$');
                    i++;
                    break;
                default:
                    i += print_special_case(string + i + 1);
                    break;
            }
        } else if (letter <= 32 || letter == '#') {
            printf("\\%03d", letter);
        } else {
            putchar(letter);
        }
    }
}

void define_substring() {
    printf("jump -substring-end\n");
    printf("label substring\n");
    printf("defvar LF@j\n");
    printf("defvar LF@i\n");
    printf("defvar LF@s\n");
    printf("pops LF@j\n");
    printf("pops LF@i\n");
    printf("pops LF@s\n");
    printf("defvar LF@s_len\n");
    printf("strlen LF@s_len LF@s\n");
    printf("defvar LF@i_less_than_zero\n");
    printf("defvar LF@j_less_than_zero\n");
    printf("defvar LF@i_greater_than_j\n");
    printf("defvar LF@i_strlen_cmp\n");
    printf("defvar LF@j_strlen_cmp\n");
    printf("lt LF@i_less_than_zero LF@i int@0\n");
    printf("lt LF@j_less_than_zero LF@j int@0\n");
    printf("gt LF@i_greater_than_j LF@i LF@j\n");
    printf("lt LF@i_strlen_cmp LF@i LF@s_len\n");
    printf("not LF@i_strlen_cmp LF@i_strlen_cmp\n");
    printf("lt LF@j_strlen_cmp LF@j LF@s_len\n");
    printf("not LF@j_strlen_cmp LF@j_strlen_cmp\n");
    printf("defvar LF@cmp_result\n");
    printf("and LF@cmp_result LF@i_less_than_zero LF@j_less_than_zero\n");
    printf("and LF@cmp_result LF@cmp_result LF@i_greater_than_j\n");
    printf("and LF@cmp_result LF@cmp_result LF@i_strlen_cmp\n");
    printf("and LF@cmp_result LF@cmp_result LF@j_strlen_cmp\n");
    printf("jumpifeq -substring-valid-values LF@cmp_result bool@true\n");
    printf("pushs nil@nil # invalid parameters\n");
    printf("return\n");
    printf("LABEL -substring-valid-values\n");
    printf("defvar LF@resulting_string\n");
    printf("defvar LF@temp_cond\n");
    printf("defvar LF@char_holder\n");
    printf("LABEL -substring-while-loop-cond\n");
    printf("lt LF@temp_cond LF@i LF@j\n");
    printf("jumpifeq -substring-while-loop-end LF@temp_cond bool@false\n");
    printf("getchar LF@char_holder LF@s LF@i\n");
    printf("CONCAT LF@resulting_string LF@resulting_string LF@char_holder\n");
    printf("pushs LF@i\n");
    printf("pushs int@1\n");
    printf("adds\n");
    printf("pops LF@i\n");
    printf("jump -substring-while-loop-cond\n");
    printf("LABEL -substring-while-loop-end\n");
    printf("pushs LF@resulting_string\n");
    printf("return\n");
    printf("LABEL -substring-end\n");
}

void define_ord() {
    printf("jump -ord-end\n");
    printf("label ord\n");
    printf("defvar LF@str\n");
    printf("pops LF@str\n");
    printf("defvar LF@str_len\n");
    printf("strlen LF@str_len LF@str\n");
    printf("jumpifneq -ord-string-not-empty LF@str_len int@0\n");
    printf("pushs int@0\n");
    printf("return\n");
    printf("label -ord-string-not-empty\n");
    printf("STRI2INT LF@str LF@str int@0\n");
    printf("pushs LF@str\n");
    printf("return\n");
    printf("label -ord-end\n");
}

void define_inbuilt_functions() {
    define_substring();
    define_ord();
}

void recurse_expression(PT_Node_t* expression_tree, int* label_indexer, const char* label_prefix) {
    if (expression_tree == NULL) {
        return;
    }

    recurse_expression(expression_tree->leftChild, label_indexer, label_prefix);
    recurse_expression(expression_tree->rightSibling, label_indexer, label_prefix);

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
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@argoneval\n");
        printf("DEFVAR TF@argonetype\n");
        printf("POPS TF@argoneval\n");
        printf("TYPE TF@argonetype TF@argoneval\n");
        printf("DEFVAR TF@argtwoval\n");
        printf("DEFVAR TF@argtwotype\n");
        printf("POPS TF@argtwoval\n");
        printf("TYPE TF@argtwotype TF@argtwoval\n");
        printf("JUMPIFEQ -%s-%d TF@argtwotype TF@argonetype\n", label_prefix, *label_indexer);
        printf("PUSHS bool@false\n");
        printf("JUMP -%s-%d\n", label_prefix, *label_indexer + 1);
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer);
        printf("DEFVAR TF@result\n");
        printf("EQ TF@result TF@argtwoval TF@argoneval\n");
        printf("PUSHS TF@result\n");
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer + 1);
        *label_indexer = *label_indexer + 2;
    } else if (get_discriminant(expression_tree) == notEqOper) {
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@argoneval\n");
        printf("DEFVAR TF@argonetype\n");
        printf("POPS TF@argoneval\n");
        printf("TYPE TF@argonetype TF@argoneval\n");
        printf("DEFVAR TF@argtwoval\n");
        printf("DEFVAR TF@argtwotype\n");
        printf("POPS TF@argtwoval\n");
        printf("TYPE TF@argtwotype TF@argtwoval\n");
        printf("JUMPIFEQ -%s-%d TF@argtwotype TF@argonetype\n", label_prefix, *label_indexer);
        printf("PUSHS bool@true\n");
        printf("JUMP -%s-%d\n", label_prefix, *label_indexer + 1);
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer);
        printf("DEFVAR TF@result\n");
        printf("EQ TF@result TF@argtwoval TF@argoneval\n");
        printf("PUSHS TF@result\nNOTS\n");
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer + 1);
        *label_indexer = *label_indexer + 2;
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
    } else if (get_discriminant(expression_tree) == nullToInt) {
        printf("POPS GF@VOID\nPUSHS int@0\n");
    } else if (get_discriminant(expression_tree) == nullToBool) {
        printf("POPS GF@VOID\nPUSHS bool@false\n");
    } else if (get_discriminant(expression_tree) == nullToReal) {
        printf("POPS GF@VOID\nPUSHS float@0x0p+0\n");
    } else if (get_discriminant(expression_tree) == nullToString) {
        printf("POPS GF@VOID\nPUSHS string@0\n");
    }
}

error(none) run_expression(PT_Node_t *expression_start, int* label_indexer, const char* label_prefix) {
    if (expression_start == NULL) {
        return_none();
    }
    PT_Node_ptr current = expression_start->leftChild;
    if (current->data.isTerminal == false) {
        return_error(ERROR_GEN_UNSPECIFIED, none);
    }

    recurse_expression(current->leftChild, label_indexer, label_prefix);
    recurse_expression(current->rightSibling, label_indexer, label_prefix);

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
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@argoneval\n");
        printf("DEFVAR TF@argonetype\n");
        printf("POPS TF@argoneval\n");
        printf("TYPE TF@argonetype TF@argoneval\n");
        printf("DEFVAR TF@argtwoval\n");
        printf("DEFVAR TF@argtwotype\n");
        printf("POPS TF@argtwoval\n");
        printf("TYPE TF@argtwotype TF@argtwoval\n");
        printf("JUMPIFEQ -%s-%d TF@argtwotype TF@argonetype\n", label_prefix, *label_indexer);
        printf("PUSHS bool@false\n");
        printf("JUMP -%s-%d\n", label_prefix, *label_indexer + 1);
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer);
        printf("DEFVAR TF@result\n");
        printf("EQ TF@result TF@argtwoval TF@argoneval\n");
        printf("PUSHS TF@result\n");
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer + 1);
        *label_indexer = *label_indexer + 2;
    } else if (get_discriminant(current) == notEqOper) {
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@argoneval\n");
        printf("DEFVAR TF@argonetype\n");
        printf("POPS TF@argoneval\n");
        printf("TYPE TF@argonetype TF@argoneval\n");
        printf("DEFVAR TF@argtwoval\n");
        printf("DEFVAR TF@argtwotype\n");
        printf("POPS TF@argtwoval\n");
        printf("TYPE TF@argtwotype TF@argtwoval\n");
        printf("JUMPIFEQ -%s-%d TF@argtwotype TF@argonetype\n", label_prefix, *label_indexer);
        printf("PUSHS bool@true\n");
        printf("JUMP -%s-%d\n", label_prefix, *label_indexer + 1);
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer);
        printf("DEFVAR TF@result\n");
        printf("EQ TF@result TF@argtwoval TF@argoneval\n");
        printf("PUSHS TF@result\nNOTS\n");
        printf("LABEL -%s-%d\n", label_prefix, *label_indexer + 1);
        *label_indexer = *label_indexer + 2;
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
    } else if (get_discriminant(current) == nullToInt) {
        printf("POPS GF@VOID\nPUSHS int@0\n");
    } else if (get_discriminant(current) == nullToBool) {
        printf("POPS GF@VOID\nPUSHS bool@false\n");
    } else if (get_discriminant(current) == nullToReal) {
        printf("POPS GF@VOID\nPUSHS float@0x0p+0\n");
    } else if (get_discriminant(current) == nullToString) {
        printf("POPS GF@VOID\nPUSHS string@0\n");
    }

    return_none();
}

error(none) print_if(PT_Node_t* if_start, int* label_indexer, const char* label_prefix, vec_context* context, bool returns_void);
error(none) print_while(PT_Node_t* while_start, int* label_indexer, const char* label_prefix, vec_context* context, bool returns_void);
error(none) call_function(PT_Node_t *call_start, int* labeler, const char* label_prefix);
error(vec_context) scan_body_for_args(PT_Node_t* body_node, vec_context* context);

error(vec_context) scan_if_for_args(PT_Node_t * if_node, vec_context* context) {
    skip_to_next_nonTerminal(if_node, BODY);
    PT_Node_t* body1 = if_node;

    if_node = if_node->rightSibling;
    skip_to_next_nonTerminal(if_node, BODY);

    PT_Node_t* body2 = if_node;

    get_value(vec_context, body1_vars, scan_body_for_args(body1->leftChild, context), vec_context);
    get_value(vec_context, body2_vars, scan_body_for_args(body2->leftChild, context), vec_context);

    vec_context new_vars = new_vec_context();

    for (size_t i = 0; i < vec_context_len(&body1_vars); ++i) {
        vec_context_insert_sorted(&new_vars, body1_vars.data[i]);
    }

    for (size_t i = 0; i < vec_context_len(&body2_vars); ++i) {
        vec_context_insert_sorted(&new_vars, body2_vars.data[i]);
    }

    return_value(new_vars, vec_context);
}

error(vec_context) scan_while_for_args(PT_Node_t* while_node, vec_context* context) {
    skip_to_next_nonTerminal(while_node, BODY);

    return scan_body_for_args(while_node->leftChild, context);
}

error(vec_context) scan_body_for_args(PT_Node_t* body_node, vec_context* context) {
    vec_context new_variables = new_vec_context();
    if (body_node == NULL) {
        return_value(new_variables, vec_context);
    }

    if (is_non_terminal(body_node, STATEMENT) == false) {
        return_error(ERROR_GEN_UNSPECIFIED, vec_context);
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
                get_value(vec_context, inner_variables, scan_if_for_args(statement, context), vec_context);
                for (size_t i = 0; i < vec_context_len(&inner_variables); i++) {
                    vec_context_insert_sorted(&new_variables,inner_variables.data[i] );
                }

                vec_context_destroy(&inner_variables);
                break;
            }

            case whileT: {
                get_value(vec_context, inner_variables, scan_while_for_args(statement, context), vec_context);
                for (size_t i = 0; i < vec_context_len(&inner_variables); i++) {
                    vec_context_insert_sorted(&new_variables,inner_variables.data[i] );
                }

                vec_context_destroy(&inner_variables);
                break;
            }

            case identOfVar: {
                const char* variable_name = extract_data(statement, string);
                if (vec_context_find_sorted(context, (char_ptr)variable_name) == false) {
                    vec_context_insert_sorted(context, (char_ptr)variable_name);
                    vec_context_insert_sorted(&new_variables, (char_ptr)variable_name);
                }
                break;
            }
            default:
                break;
        }
    }

    return_value(new_variables, vec_context);
}

error(none) print_body_without_new_context(PT_Node_t* body_node, int* label_indexer, const char* label_prefix ,vec_context* context, bool returns_void, bool is_function_body) {
    if (body_node == NULL) {
        // Void functions contain implicit returns
        if (returns_void && is_function_body) {
            printf("PUSHS nil@nil\n");
            printf("RETURN\n");
        }
        return_none();
    }

    get_value(vec_context, new_vars, scan_body_for_args(body_node, context), none);
    for (size_t i = 0; i < vec_context_len(&new_vars); ++i) {
        printf("DEFVAR LF@%s\n", new_vars.data[i]);
    }

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
                error(none) error = print_if(statement, label_indexer, label_prefix, context, returns_void);
                if (is_error(error)) {
                    vec_context_destroy(context);
                    forward_error(error, none);
                }
                break;
            }
            case returnT: {
                skip_to_next_nonTerminal(statement, RET_VAL);

                if (statement->leftChild != NULL) {
                    error(none) result = run_expression(statement->leftChild, label_indexer, label_prefix);
                    if (is_error(result)) {
                        vec_context_destroy(context);
                        forward_error(result, none);
                    }
                }

                if (returns_void) {
                    printf("PUSHS nil@nil\n");
                }
                printf("RETURN\n");
                break;
            }

            case whileT: {
                error(none) continued = print_while(statement, label_indexer, label_prefix, context, returns_void);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }
                break;
            }
            case identOfFunct: {
                error(none) continued = call_function(statement, label_indexer, label_prefix);
                if (is_error(continued)) {
                    vec_context_destroy(context);
                    forward_error(continued, none);
                }

                // If the function returns a value it will be on the stack, we aren't saving the value, so we void it
                printf("POPS GF@VOID\n");
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
                    run_expression(statement->leftChild, label_indexer, label_prefix);

                } else if (is_terminal(statement->leftChild, identOfFunct)) {
                    call_function(statement->leftChild, label_indexer, label_prefix);
                }

                printf("POPS LF@%s\n", variable_name);
                break;
            }
            default:
                // No other terminal type should be encountered here
                return_error(ERROR_GEN_UNSPECIFIED, none);
        }
    }

    // Void functions contain implicit returns
    if (returns_void && is_function_body) {
        printf("PUSHS nil@nil\n");
        printf("RETURN\n");
    }


    return_none();
}

error(none) print_body(PT_Node_t* body_node, int* label_indexer, const char* label_prefix ,vec_context* outer_context, bool returns_void, bool is_function_body) {
    get_value(vec_context, context , clone_context_vec(outer_context), none);

    error(none) result = print_body_without_new_context(body_node, label_indexer, label_prefix, &context, returns_void, is_function_body);
    if (is_error(result)) {
        vec_context_destroy(&context);
    }

    return result;
}


error(none) print_if(PT_Node_t* if_start, int* label_indexer, const char* label_prefix, vec_context* context, bool returns_void) {
    skip_to_next_nonTerminal(if_start, EXPR);
    error(none) result = run_expression(if_start, label_indexer, label_prefix);
    if (is_error(result)) {
        forward_error(result, none);
    }

    int index_if_block = *label_indexer;
    int index_else_block = *label_indexer + 1;
    int index_end_block = *label_indexer + 2;
    *label_indexer = *label_indexer + 3;

    // Figure out the resulting type of the expression
    printf("POPS GF@RAX\nTYPE GF@RBX GF@RAX\n");
    // Use the proper comparison function
    printf("JUMPIFEQ -%s-%d GF@RBX string@string\n", label_prefix, *label_indexer);
    printf("JUMPIFEQ -%s-%d GF@RBX string@int\n", label_prefix, *label_indexer + 1);
    printf("JUMPIFEQ -%s-%d GF@RBX string@float\n", label_prefix, *label_indexer + 2);
    printf("JUMPIFEQ -%s-%d GF@RBX string@bool\n", label_prefix, *label_indexer + 3);
    printf("JUMPIFEQ -%s-%d GF@RBX string@nil\n", label_prefix, index_else_block);

    printf("LABEL -%s-%d\nJUMPIFEQ -%s-%d GF@RAX string@0\n",label_prefix, *label_indexer, label_prefix, index_else_block);
    printf("STRLEN GF@RAX GF@RAX\n");

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
        print_body_without_new_context(if_start->leftChild, label_indexer, label_prefix ,context, returns_void, false);
    }

    printf("JUMP -%s-%d\n", label_prefix, index_end_block);

    // else block
    printf("LABEL -%s-%d\n", label_prefix, index_else_block);

    if_start = if_start->rightSibling; // skip_to_next... skip_to_next would just return the current body token if we didn't move to the right
    skip_to_next_nonTerminal(if_start, BODY);

    if (if_start->leftChild != NULL) {
        print_body_without_new_context(if_start->leftChild, label_indexer, label_prefix, context, returns_void, false);
    }

    // exit
    printf("LABEL -%s-%d\n", label_prefix, index_end_block);

    return_none();
}

error(none) print_while(PT_Node_t* while_start, int* label_indexer, const char* label_prefix, vec_context* context, bool returns_void) {
    skip_to_next_nonTerminal(while_start, EXPR)

    int index_while_if_block = *label_indexer;
    int index_while_block = *label_indexer + 1;
    int index_end_block = *label_indexer + 2;
    *label_indexer = *label_indexer + 3;

    printf("LABEL -%s-%d\n", label_prefix, index_while_if_block);
    error(none) result = run_expression(while_start, label_indexer, label_prefix);
    if (is_error(result)) {
        forward_error(result, none);
    }

    // Figure out the resulting type of the expression
    printf("POPS GF@RAX\nTYPE GF@RBX GF@RAX\n");
    // Use the proper comparison function
    printf("JUMPIFEQ -%s-%d GF@RBX string@string\n", label_prefix, *label_indexer);
    printf("JUMPIFEQ -%s-%d GF@RBX string@int\n", label_prefix, *label_indexer + 1);
    printf("JUMPIFEQ -%s-%d GF@RBX string@float\n", label_prefix, *label_indexer + 2);
    printf("JUMPIFEQ -%s-%d GF@RBX string@bool\n", label_prefix, *label_indexer + 3);
    printf("JUMPIFEQ -%s-%d GF@RBX string@nil\n", label_prefix, index_end_block);

    printf("LABEL -%s-%d\nJUMPIFEQ -%s-%d GF@RAX string@0\n",label_prefix, *label_indexer, label_prefix, index_end_block);
    printf("STRLEN GF@RAX GF@RAX\n");

    // int comparison function
    printf("LABEL -%s-%d\nJUMPIFNEQ -%s-%d GF@RAX int@0\nJUMP -%s-%d\n", label_prefix, *label_indexer + 1, label_prefix, index_while_block, label_prefix, index_end_block);

    // float comparison function
    printf("LABEL -%s-%d\nJUMPIFNEQ -%s-%d GF@RAX float@0x0p+0\nJUMP -%s-%d\n", label_prefix, *label_indexer + 2, label_prefix, index_while_block, label_prefix, index_end_block);

    // We can just fall through this part instead of jumping to the while block
    // bool comparison function
    printf("LABEL -%s-%d\nJUMPIFEQ -%s-%d GF@RAX bool@false\n", label_prefix, *label_indexer + 3, label_prefix, index_end_block);
    *label_indexer = *label_indexer + 4;

    // while block
    printf("LABEL -%s-%d\n", label_prefix, index_while_block);
    skip_to_next_nonTerminal(while_start, BODY);
    print_body_without_new_context(while_start->leftChild, label_indexer, label_prefix ,context, returns_void, false);
    printf("JUMP -%s-%d\n", label_prefix, index_while_if_block);
    printf("LABEL -%s-%d\n", label_prefix, index_end_block);


    return_none();
}

error(none) print_call_args(PT_Node_t * args) {
    while (args != NULL) {
        if (is_non_terminal(args->leftChild, TERM)) {
            if (get_discriminant(args->leftChild->leftChild) == nullT) {
                printf("PUSHS nil@nil\n");
            } else if (get_discriminant(args->leftChild->leftChild) == decNum) {
                double value = extract_data(args->leftChild->leftChild, decNuber);
                printf("PUSHS float@%a\n", value);
            } else if (get_discriminant(args->leftChild->leftChild) == integer) {
                int value = extract_data(args->leftChild->leftChild, integer);
                printf("PUSHS int@%d\n", value);
            } else if (get_discriminant(args->leftChild->leftChild) == string) {
                const char* value = extract_data(args->leftChild->leftChild, string);
                printf("PUSHS string@");
                print_normalized_string(value);
                printf("\n");
            }
        } else {
            if (is_terminal(args->leftChild, identOfVar) == false) {
                return_error(ERROR_GEN_UNSPECIFIED, none);
            }
            printf("PUSHS LF@%s\n", extract_data(args->leftChild, string));
        }

        skip_to_next_nonTerminal(args, ARGS_NEXT);
        args = args->leftChild;
        if (args == NULL) {
            break;
        } else {
            skip_to_next_nonTerminal(args, ARG_TYPE);
        }
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

error(none) call_function(PT_Node_t *call_start, int* labeler, const char* label_prefix) {
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
        printf("PUSHS nil@nil\n");
        return_none();
    } else if (strcmp(function_name, "floatval") == 0) {
        print_call_args(call_start->leftChild);
        printf("POPS GF@RBX\nTYPE GF@RAX GF@RBX\nPUSHS GF@RBX\n");
        printf("JUMPIFEQ -%s-%d GF@RAX string@float\n", label_prefix, *labeler);
        printf("JUMPIFEQ -%s-%d GF@RAX string@int\n", label_prefix, (*labeler) + 1);
        printf("JUMPIFEQ -%s-%d GF@RAX string@nil\n", label_prefix, (*labeler) + 2);
        printf("EXIT int@7\n");
        printf("LABEL -%s-%d\nint2floats\nJUMP -%s-%d\n", label_prefix, (*labeler) + 1, label_prefix, (*labeler));
        printf("LABEL -%s-%d\nPOPS GF@RBX\nPUSHS float@0x0p+0\n", label_prefix, (*labeler) + 2);
        printf("LABEL -%s-%d\n", label_prefix, (*labeler));
        *labeler = (*labeler) + 3;

        return_none();
    } else if (strcmp(function_name, "intval") == 0) {
        print_call_args(call_start->leftChild);
        printf("POPS GF@RBX\nTYPE GF@RAX GF@RBX\nPUSHS GF@RBX\n");
        printf("JUMPIFEQ -%s-%d GF@RAX string@int\n", label_prefix, *labeler);
        printf("JUMPIFEQ -%s-%d GF@RAX string@float\n", label_prefix, (*labeler) + 1);
        printf("JUMPIFEQ -%s-%d GF@RAX string@nil\n", label_prefix, (*labeler) + 2);
        printf("EXIT int@7\n");
        printf("LABEL -%s-%d\nfloat2ints\nJUMP -%s-%d\n", label_prefix, (*labeler) + 1, label_prefix, (*labeler));
        printf("LABEL -%s-%d\nPOPS GF@RBX\nPUSHS int@0\n", label_prefix, (*labeler) + 2);
        printf("LABEL -%s-%d\n", label_prefix, (*labeler));
        *labeler = (*labeler) + 3;

        return_none();
    } else if (strcmp(function_name, "strval") == 0) {
        print_call_args(call_start->leftChild);
        printf("POPS GF@RBX\nTYPE GF@RAX GF@RBX\nPUSHS GF@RBX\n");
        printf("JUMPIFEQ -%s-%d GF@RAX string@string\n", label_prefix, *labeler);
        printf("JUMPIFEQ -%s-%d GF@RAX string@nil\n", label_prefix, (*labeler) + 1);
        printf("EXIT int@7\n");
        printf("LABEL -%s-%d\nPOPS GF@RBX\nPUSHS string@\n", label_prefix, (*labeler) + 1);
        printf("LABEL -%s-%d\n", label_prefix, (*labeler));
        *labeler = (*labeler) + 2;

        return_none();
    } else if (strcmp(function_name, "strlen") == 0) {
        print_call_args(call_start->leftChild);
        printf("POPS GF@RAX\nSTRLEN GF@RAX GF@RAX\nPUSHS GF@RAX\n");

        return_none();
    } else if (strcmp(function_name, "chr") == 0) {
        print_call_args(call_start->leftChild);
        printf("INT2CHARS\n");

        return_none();
    }

    if (call_start->leftChild != NULL) {
        print_call_args(call_start->leftChild);
    }

    printf("CREATEFRAME\nPUSHFRAME\nCALL %s\n", function_name);

    printf("POPFRAME\n");
    return_none();
}

error(none) load_function_args(PT_Node_t* args, vec_context* context) {
    if (args == NULL) {
        return_none();
    }

    skip_to_next_nonTerminal(args, TYPE);
    args = args->rightSibling;
    const char* var_name = extract_data(args, string);
    skip_to_next_nonTerminal(args, PARAMS_NEXT);

    load_function_args(args->leftChild, context);

    vec_context_insert_sorted(context, (char_ptr)var_name);
    printf("DEFVAR LF@%s\nPOPS LF@%s\n", var_name, var_name);
    return_none();
}

error(none) define_function(PT_Node_t *function_node, int* label_indexer) {
    PT_Node_ptr current = function_node->rightSibling;
    const char* function_name = extract_data(current, string);

    // context keeps track of which variable have already been defined
    vec_context context = new_vec_context();

    printf("JUMP %s-end\n", function_name);
    printf("LABEL %s\n", function_name);

    // Skip to function parameters
    skip_to_next_nonTerminal(current, PARAMS);
    load_function_args(current->leftChild, &context);

    skip_to_next_nonTerminal(current, TYPE);
    bool returns_void = false;
    if (is_terminal(current->leftChild, identOfType)) {
        const char* type = extract_data(current->leftChild, string);
        if (strcmp(type, "void") == 0) {
            returns_void = true;
        }
    }

    skip_to_next_nonTerminal(current, BODY);

    print_body(current->leftChild, label_indexer, function_name, &context, returns_void, true);

    printf("LABEL %s-end\n", function_name);
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
    printf("DEFVAR GF@RAX\nDEFVAR GF@RBX\nDEFVAR GF@VOID\n");

    define_inbuilt_functions();
    printf("#start of code\n");

    // Main printer loop
    while (current != NULL) {

        current = current->leftChild;
        if (is_non_terminal(current, EXPR)) {
            skip_to_next_nonTerminal(current, PROG);
            continue;
        }

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
                get_value(vec_context, new_vars, scan_if_for_args(current, &context), none);
                for (size_t i = 0; i < vec_context_len(&new_vars); i++) {
                    printf("DEFVAR LF@%s\n", new_vars.data[i]);
                }

                print_if(current, &label_indexer, "", &context, false);
                break;
            }
            case returnT: {
                skip_to_next_nonTerminal(current, RET_VAL);

                // Expressions are pure functions and their result is not observed when returning from the main program body
                // Therefore we can safely ignore them
                printf("EXIT int@0\n");
                break;
            }

            case whileT: {
                get_value(vec_context, new_vars, scan_while_for_args(current, &context), none);
                for (size_t i = 0; i < vec_context_len(&new_vars); i++) {
                    printf("DEFVAR LF@%s\n", new_vars.data[i]);
                }
                print_while(current, &label_indexer, "", &context, false);
                break;
            }
            case identOfFunct: {
                call_function(current, &label_indexer, "");
                // If the function returns a value it will be on the stack, we aren't saving the value, so we void it
                printf("POPS GF@VOID\n");
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
                    run_expression(current->leftChild, &label_indexer, "");

                } else if (is_terminal(current->leftChild, identOfFunct)) {
                    call_function(current->leftChild, &label_indexer, "");
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