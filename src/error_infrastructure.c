#include "headers/error_infrastructure.h"

#include "stdio.h"

void print_err_info(ERR_INFO_ err_info) {
    fprintf(stderr, "%s:%d encountered error in function %s of kind: %s\n", err_info._file, err_info._line, err_info._func, error_kind_name(err_info._kind));
    fprintf(stderr, "%s\n", error_kind_message(err_info._kind));
}