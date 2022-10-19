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
        case ERROR_LEX: //
            s(ERROR_LEX)
        case ERROR_LEX_VAR:
            s(ERROR_LEX_VAR);
        case ERROR_LEX_DTYPE:
            s(ERROR_LEX_DTYPE);
        case ERROR_LEX_ALPHNUM:
            s(ERROR_LEX_ALPHNUM);
        case ERROR_LEX_INVSTR:
            s(ERROR_LEX_INVSTR);
        case ERROR_LEX_EOFCOMM:
            s(ERROR_LEX_EOFCOMM);
        case ERROR_LEX_PROLOG:
            s(ERROR_LEX_PROLOG);
        case ERROR_LEX_EOFCHAR:
            s(ERROR_LEX_EOFCHAR);
        case ERROR_LEX_NOTABLE:
            s(ERROR_LEX_NOTABLE);
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
        case ERROR_LEX:
            return "Scaner encountered invalid sequence of characters";
        case ERROR_LEX_VAR:
            return "Scaner: Following character after '$' is number";
        case ERROR_LEX_DTYPE:
            return "Scaner: Following character after '?' wasn't identificator of data type";
        case ERROR_LEX_ALPHNUM:
            return "Scaner: Scaner encoutered letter right after number";
        case ERROR_LEX_INVSTR:
            return "Scaner: Invalid character in string";
        case ERROR_LEX_EOFCOMM:
            return "Scaner: Unterminated multiline commnet";
        case ERROR_LEX_PROLOG:
            return "Scaner: Invalid prolog format";
        case ERROR_LEX_EOFCHAR:
            return "Scanner: Scaner found character after \"?>\" token";
        case ERROR_LEX_NOTABLE:
            return "Scanner: Table is NULL pointer";
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
        case ERROR_LEX_VAR:
        case ERROR_LEX_DTYPE:
        case ERROR_LEX_ALPHNUM:
        case ERROR_LEX_EOFCOMM:
        case ERROR_LEX_INVSTR:
        case ERROR_LEX_PROLOG:
        case ERROR_LEX_EOFCHAR:
        case ERROR_LEX_NOTABLE:
        case ERROR_LEX:
            return 1;
        default:
            return 99;
    }
}