#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scaner.h"


bool resizeString ( char** string , unsigned int* size)
{
    *size *= 2;
    char* temp = ( char* ) realloc( *string , *size);

    if ( temp == NULL )
    {
        return false;
    }

    *string = temp;
    return true;
}

bool finalResize ( char** string)
{
    unsigned int size = strlen ( *string );

    char* temp = ( char* ) realloc ( *string , size + 1 );

    if ( temp == NULL )
    {
        return false;
    }
    *string = temp;

    return true;
}


tokenType firstState ( int character )
{
    if ( character == '$') return identOfVar;
    else if ( isalpha( character ) || character == '_') return identifier;
    else if ( character == '?' ) return identOfType;
    else if ( isdigit( character ) ) return integer;
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
    bool endState = false;
    int character = 0;
    int counter = 1 ;
    unsigned int size = DEFAULT_SIZE;
    tokenType state = def;

    token* newToken = ( token* ) malloc( sizeof( token ) ) ;
    char* info = ( char* ) calloc ( DEFAULT_SIZE , sizeof( char ) );

    if( newToken == NULL || info == NULL)
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

    newToken->discriminant = state = firstState( character );

    if ( state == def ){

        return NULL;  //TODO plus that error thing
    }

    info[0] = character ;

    while( true )
    {
        character == getc( stdin );

        //TODO check if char is EOF and send current token away if possible

        switch ( state )
        {
            case identOfVar :
                            if ( counter == 1 && ( isalpha( character ) || character == '_'))
                            {
                                info [ counter ] = character;
                                counter++;
                                break;
                            }
                            else if ( counter > 1 && ( isalnum( character ) || character == '_') )
                            {
                                info [ counter ] = character ;
                                endState = true;
                                counter++;
                                break;
                            }

                            if ( endState )
                            {
                                if ( finalResize( &info ) )
                                {
                                    free( info );
                                    free( newToken );
                                    return NULL; //TODO thet error thing
                                }
                                ungetc ( character , stdin );
                                newToken->info.string = info;

                                return newToken;
                            }

                            free(info);
                            free(newToken);
                            return NULL ; // TODO that error thing 

            case identifier :
            case identOfType :
            case integer :
            case string :
            case lineComment :
            case plusSign :
            case minusSign :
            case concatenation :
            case lessOper :
            case moreOper :
            case EqOper :
            case colon :
            case semicolon :
            case comma : 
            case openParen :
            case openSetParen :
            case closeParen :
            case closeSetParen :
            default: break;
        }

        if ( counter == size )
        {
            if( resizeString ( &info , &size ) )
            {
                free(info);
                free(newToken);
                return NULL; //TODO that erro thing 
            }
        }

    }

}
