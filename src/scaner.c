#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scaner.h"





tokenType firstState(int character )
{
    // if else chain
}



token* getToken()
{

    int character = 0;
    tokenType state = def;

    token* newToken = ( token* ) malloc( sizeof( token ) * DEFAULT_SIZE ) ;

    if( newToken == NULL )
    {
        return NULL; //TODO plus that error thing which adam created
    }

    // discards whitespace characters 
    while( isspace( character = getc( stdin ) ) && character != EOF );

    if( character == EOF )
    {
        newToken->discriminant = endOfFile;
        newToken->info.integer = -1 ;

        return newToken;
    }

    

}