/****************************************************************
 * @name scanner.h
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdbool.h>
#include "IFJ_2022.h"
#include "error.h"
#include "interner.h"

#define DEFAULT_SIZE 20
#define NUM_OF_KWORDS 11
#define NUM_OF_DTYPES 3
#define LEN_OF_PROLSYM 6
#define LEN_OF_DECLARE 7
#define LEN_OF_STRICT 12


bool resizeString(char** string , unsigned int *size ) ;

bool finalResize ( char** string ,unsigned int  size) ;

char * findDot( char* info ) ;

bool mapStringToKeyword(const char* str, tokenType* token);

bool isStringAType(const char* str);

tokenType checkForKeyword ( char* string ) ;

token_t* convertNum ( token_t* newToken , char* info ) ;

tokenType firstState( int character ) ;

error(token_ptr) getToken () ;

error(none) returnToken ( token_ptr retToken ) ;

int checkProlog () ;

#endif