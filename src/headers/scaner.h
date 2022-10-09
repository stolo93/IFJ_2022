/****************************************************************
 * @name scanner.h
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdbool.h>
#include "tokenStruct.h"

#define DEFAULT_SIZE 20
#define NUM_OF_KWORDS 11


bool resizeString(char** string , unsigned int* size ) ;

bool finalResize ( char** string) ;

char * findDot( char* info ) ;

tokenType checkForKeyword ( char* string ) ;

token* convertNum ( token* newToken , char* info ) ;

tokenType firstState( int character ) ;

token* getToken () ;

bool returnToken ( token* retToken ) ;

bool checkProlog () ;