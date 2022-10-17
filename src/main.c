#include "headers/IFJ_2022.h"
#include "headers/error_infrastructure.h"

// All code that would go into main goes here
// Please change the name to a better one
error(none) real_main(int argc, char** argv) {

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