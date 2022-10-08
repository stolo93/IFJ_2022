#include "headers/error.h"

#define s(token) return #token;

const char* error_kind_name(enum error_kinds kind) {
    switch (kind) {
        case NOERR:
            s(NOERR)
        case UNKNOWN:
            s(UNKNOWN)
        case INVALID_VAL:
            s(INVALID_VAL)
        default:
            return "ERROR KIND WAS NOT GIVEN A NAME PLEASE CHANGE";
    }
}
const char* error_kind_message(enum error_kinds kind) {
    switch (kind) {
        case NOERR:
            return "No error occurred";
        case UNKNOWN:
            return "An unknown condition occurred";
        case INVALID_VAL:
            return "An invalid value was encountered";
        default:
            return "KIND WAS NOT GIVEN A ERROR MESSAGE!!!";
    }
}

int error_kind_exit_code(enum error_kinds kind) {
    switch (kind) {
        case NOERR:
            return 0;
        case UNKNOWN:
            return 99;
        case INVALID_VAL:
            return 99;
        default:
            return 99;
    }
}