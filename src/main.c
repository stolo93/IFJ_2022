#include <stdio.h>
#include "headers/IFJ_2022.h"
#include "headers/error_infrastructure.h"
#include "headers/interner.h"

error( interner ) init;
interner* interner_ptr;


// All code that would go into main goes here
// Please change the name to a better one
error(none) real_main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    init = interner_new_with_capacity( DEFAULT_SIZE_INTER );
    if( is_error( init ))
    {
        forward_error( init , none );
    }
    interner_ptr = &init._value;

    interner_destroy( interner_ptr);
    

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
