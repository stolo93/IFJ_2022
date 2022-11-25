#ifndef IFJ_2022_ERROR_H
#define IFJ_2022_ERROR_H

// From C99
#include "stdbool.h"

// If you discover a way to construct a value in a return statement please inform me

/**
 * Kinds of errors this application might encounter
 */
enum error_kinds {
    /* No error */
    NOERR,

    /* Undefinable error */
    UNKNOWN,

    /* Generic invalid value error */
    INVALID_VAL,

    /* Malloc failed to alocate memory*/
    ERROR_MAL,

    /* Function was called with NULL pointer */
    ERROR_HTAB_INVPTR,
 
    /* Scaner encountered invalid token */
    ERROR_LEX, 

    /* Function didn't find entry with matching key */
    ERROR_HTAB_NKEY,

    /* Table has invalid bucket pointer */
    ERROR_HTAB_INVBPTR,

    /* Parameter num must be greater or equal to 1 */
    ERROR_HTAB_INIT,

    /* Function was called with NULL pointer or newn was less then 1 */
    ERROR_HTAB_RESZ,

    /* Vector was empty */
    VECTOR_EMPTY_ERROR,

    /* Attempted to index out of bounds of a vector */
    VECTOR_INDEX_BOUNDS_ERROR,

    /* Vector failed to reallocate */
    VECTOR_REALLOC_ERROR,

    /* Following character after '$' was number */
    ERROR_LEX_VAR,

    /* Following character after '?' wasn't identificator of data type */
    ERROR_LEX_DTYPE,

    /* Scaner encoutered letter right after number */
    ERROR_LEX_ALPHNUM,

    /* Invalid character in string */
    ERROR_LEX_INVSTR,

    /* Unterminated multiline comment */
    ERROR_LEX_EOFCOMM,

    /* Invalid prolog */
    ERROR_LEX_PROLOG,

    /* Character after EOF token */
    ERROR_LEX_EOFCHAR,

    ERROR_LEX_NOTABLE,

    ERROR_SYNTAX,

    /* Undefined function */
    ERROR_SEM_UNDEF_FUNC,

    /* Redefined function */
    ERROR_SEM_REDEF_FUNC,

    /* Invalid function call argument count */
    ERROR_SEM_FUNC_ARGCNT_CNT,

    /* Invalid function call argument type*/
    ERROR_SEM_FUNC_ARGCNT_TYPE,

    /* Invalid function return value type */
    ERROR_SEM_FUNC_RET_TYPE,

    /* Usage of undefined variable */
    ERROR_SEM_UNDEF_VAR,

    /* Invalid return value expression count */
    ERROR_SEM_RETVAL_EXPR,

    /* Type compatibility in expression error */
    ERROR_SEM_TYPE_EXPR,

    /* Other semantic errors */
    ERROR_SEM,

    ERROR_SEM_NODE,

    ERROR_SEM_EXPR,

    ERROR_SEM_TYPE,

    /* Some error occurred during code generation */
    ERROR_GEN_UNSPECIFIED,
};

const char* error_kind_name(enum error_kinds kind);
const char* error_kind_message(enum error_kinds kind);
int error_kind_exit_code(enum error_kinds kind);

typedef struct ERR_INFO_ {
    int _line;
    const char* _file;
    const char* _func;
    enum error_kinds _kind;
} ERR_INFO_;

/**
 * Defines an error of \p type
 *
 * @param type The type which this error contains as a valid value
 */
#define define_error(type) typedef struct { bool _is_err; union { ERR_INFO_ _err; __typeof__(type) _value; }; } error_ ## type ## _;

/**
 * Returns a usable error type of \p type after it's been define with error()
 */
#define error(type) __typeof__(error_ ## type ## _)

/**
 * Checks if /p error_obj is an error
 */
#define is_error(error_obj) (error_obj)._is_err

/**
 * Reads value from /p error_obj and creates a variable of type /p variable_type and name /p variable name.
 *
 * It is undefined behaviour to extract a value from an /p error_obj which contains an error
 */
#define get_value_unchecked(variable_type, variable_name, error_obj) __typeof__(variable_type) (variable_name) = (error_obj)._value

/**
 * Returns an error object containing the /p error_kind
 *
 * @param error_kind the kind of error we will return
 * @param return_error_type the type of value the error object this function returns can hold
 *
 */
#define return_error(error_kind, return_error_type) do {error(return_error_type) ERROR___RETURN_VALUE___ = {._is_err = true, ._err = {._line = __LINE__, ._file = __FILE__, ._func = __func__, ._kind = (error_kind)}}; return ERROR___RETURN_VALUE___;} while(0)

/**
 * Returns an error object containing the /p value
 *
 * @param value the value this error objects holds
 * @param return_error_type the type of /p value
 */
// We define a temporary variable, so we don't call error_obj multiple times
#define return_value(value, return_error_type) do {error(return_error_type) ERROR___RETURN_VALUE___ = {._is_err = false, ._value = (value)}; return ERROR___RETURN_VALUE___;} while(0)

/**
 * Returns an error object of type none containing an value
 */
#define return_none() do {error(none) ERROR___RETURN_VALUE___ = {._is_err = false, ._value = {}}; return ERROR___RETURN_VALUE___;} while(0)

/**
 * Takes an existing /p error_obj that holds an error and returns it
 *
 * This needs to exist because he error object we return might be able to hold a value of different type than the error object we receive
 * It is undefined behaviour to forward an /p error_obj that holds a value
 *
 * @param error_obj the error object
 * @param return_error_type the type of /p value
 */
#define forward_error(error_obj, return_error_type) do {__typeof__(error_obj) ___TEMPORARY___ = (error_obj); error_ ## return_error_type ## _ ERROR___RETURN_VALUE___ = {._is_err = true, ._err = {._line = ___TEMPORARY___._err._line, ._file = ___TEMPORARY___._err._file, ._func = ___TEMPORARY___._err._func, ._kind = ___TEMPORARY___._err._kind}}; return ERROR___RETURN_VALUE___;} while(0)

/**
 * Extracts the value from /p error_obj and saves it in a variable.
 *
 * Will return an error from the function if /p error_obj contains an error
 *
 * @param variable_type the type of the variable in which the value contained in /p error_obj is being saved
 * @param variable_name the name of the variable in which the value contained in /p error_obj is being saved
 * @param error_obj the object which we're extracting our value from
 * @param return_error_type the type of value the error object this function returns can hold
 *
 */
#define get_value(variable_type, variable_name, error_obj, return_error_type) error(variable_type) TEMPORARY_ERROR_OBJECT_ ## variable_name ## _ = (error_obj); if (is_error(TEMPORARY_ERROR_OBJECT_ ## variable_name ## _)) {forward_error(TEMPORARY_ERROR_OBJECT_ ## variable_name ## _, return_error_type);}; get_value_unchecked(variable_type, variable_name, TEMPORARY_ERROR_OBJECT_ ## variable_name ## _)

/**
 * A type representing void but valid as a complete type (can be used in struct definitions)
 *
 * error(void) doesn't produce a valid type so use error(none) instead
 */
typedef struct none {} none;

#endif //IFJ_2022_ERROR_H
