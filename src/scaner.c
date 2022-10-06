#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scaner.h"




tokenType firstState(int character )
{
    if ( character == '$') return identOfVar;
    else if ( isalpha( character ) || character == '_') return identifier;
    else if ( character == '?' ) return identOfType;
    else if ( isdigit( character ) ) return Integer;
    else if ( character == '"' ) return string;
    else if ( character == '/' ) return lineComment;
    else if ( character == '+' ) return plusSign;
    else if ( character == '-' ) return minusSign;
    else if ( character == '*' ) return multiply;
    else if ( character == '.' ) return concatenation;
    else if ( character == '<' ) return lessOper;
    else if ( character == '>' ) return moreOper;
    else if ( character == '=' ) return EqOper;
    else if ( character == ':' ) return colon;
    else if ( character == ';' ) return semicolon;
    else if ( character == ',' ) return comma;
    else if ( character == '(' ) return openParen;
    else if ( character == ')' ) return closeParen;
    else if ( character == '{' ) return openSetParen;
    else if ( character == '}' ) return closeSetParen;
    else return def;
    
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

    
    state = firstState( character );
}