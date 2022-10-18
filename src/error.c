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
        case ERROR_MAL:
            s(ERROR_MAL);
        case ERROR_HTAB_INVPTR:
            s(ERROR_HTAB_INVPTR);
        case ERROR_HTAB_INVBPTR:
            s(ERROR_HTAB_INVBPTR);
        case ERROR_HTAB_INIT:
            s(ERROR_HTAB_INIT);
        case ERROR_HTAB_RESZ:
            s(ERROR_HTAB_RESZ);
        case VECTOR_EMPTY_ERROR:
            s(VECTOR_EMPTY_ERROR)
        case VECTOR_REALLOC_ERROR:
            s(VECTOR_REALLOC_ERROR)
        case VECTOR_INDEX_BOUNDS_ERROR:
            s(VECTOR_INDEX_BOUNDS_ERROR)
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
        case ERROR_MAL:
            return "Malloc failed to alocate memory";
        case ERROR_HTAB_INVPTR:
            return "Htab: Function was called with NULL pointer";
        case ERROR_HTAB_INVBPTR:
            return "Htab: Table has invalid bucket pointer";
        case ERROR_HTAB_INIT:
            return "Htab: Parameter num must be greater or equal to 1";
        case ERROR_HTAB_RESZ:
            return "Htab: Function was called with NULL pointer or newn was less then 1";
        case VECTOR_EMPTY_ERROR:
            return "Attempted to access a value in an empty vector";
        case VECTOR_REALLOC_ERROR:
            return "Failed to reallocate a vector's inner buffer";
        case VECTOR_INDEX_BOUNDS_ERROR:
            return "Attempted to access a value out of bounds of the vector";
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
        case ERROR_MAL:
            return 99;
        case ERROR_HTAB_INVPTR:
        case ERROR_HTAB_INVBPTR:
        case ERROR_HTAB_INIT:
        case ERROR_HTAB_RESZ:
        case VECTOR_EMPTY_ERROR:
        case VECTOR_REALLOC_ERROR:
        case VECTOR_INDEX_BOUNDS_ERROR:
            return 99;
        default:
            return 99;
    }
}