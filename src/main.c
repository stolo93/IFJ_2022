#include "./headers/IFJ_2022.h"
#include "./headers/error_infrastructure.h"
#include "./headers/syntactic_analysis.h"
#include "./headers/scaner.h"
#include "./headers/syntax_tree.h"

#include <stdio.h>

// All code that would go into main goes here
// Please change the name to a better one
error(none) real_main(int argc, char** argv) {
    (void) argc;
    (void) argv;


    // Create an abstract syntactic tree
    PT_Data_t prolog_node = {.isTerminal = false, .type.nonTerminal = PROLOG};
    error(PT_Node_ptr) tmp_node = PT_CreateNode(prolog_node);
    get_value(PT_Node_ptr, syntax_tree, tmp_node, none);

    // Get the first token and insert it into the s_tree
    error(token_ptr) tmp_token = getToken();
    get_value(token_ptr, token, tmp_token, none);

    PT_Data_t node_data = {.isTerminal = true, .type.terminal = token};
    tmp_node = PT_AddChild(syntax_tree, node_data);
    get_value(PT_Node_ptr, token_node, tmp_node, none);

    // Syntactic analysis
    error(_Bool) tmp_result = SA_Prolog(token_node);
    get_value(bool, result, tmp_result, none);


    // Result
    printf("%s\n", result ? "good" : "bad");

    // Deletes the whole tree
    PT_DeleteNode(&syntax_tree);
    return_none();
}

int main(int argc, char** argv) {
    error(none) error_object = real_main(argc, argv);

    if (is_error(error_object)){
        ERR_INFO_ error_info = error_object._err;

        print_err_info(error_info);

        return error_kind_exit_code(error_object._err._kind);
    }

    return 0;
}
