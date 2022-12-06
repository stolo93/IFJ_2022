/****************************************************************
 * @name scanner.c
 * @author  Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * @brief File containing functions for scanner and its proper function
 * @date 20.10.2022
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include "headers/scaner.h"
#include "headers/error.h"
#include "headers/interner.h"

extern interner* interner_ptr;
extern vec_token_ptr returnedTokens;

#define __interning() do{ id = intern( interner_ptr , info );\
                      if(  is_error( id )){\
                        forward_error( id , token_ptr );}\
                      newToken->info.string = id._value;\
                      }while ( 0 )\
/** Function for resizing strings
 *
 *  @param string which will be resized
 *  @param size pointer to current size of string
 *  @return succes of resizing
 ***/
bool resizeString ( char** string , unsigned int * size)
{
    *size *= 2;
    char* temp = ( char* ) realloc( *string , (*size) * sizeof(char));  //double the size of original string

    if ( temp == NULL )
    {
        return false;
    }

    *string = temp;
    return true;
}
/** Function for resizing string before being passed to token
 *
 *  @param string which will be resized
 *  @param size current size of string 
 *  @return succes of resizing
 ***/
bool finalResize ( char** string, unsigned int size )
{
    //unsigned int size = strlen ( *string ); //get lenght of string

    char* temp = ( char* ) realloc ( *string , (size + 1) * sizeof(char)); //make it tight as possible
    if ( temp == NULL )
    {
        return false;
    }
    *string = temp;
    (*string)[ size ] = '\0';
    return true;
}
/** Function for finding dot in string
 *  @param info string with number which may contain dot
 *  @return pointer pointing at dot in string or NULL if there was no dot in sting
 ***/
char * findDot( char* info )
{
    for(unsigned int counter = 0 ; counter < strlen( info ) ; counter++ )
    {
        if( info [ counter ] == '.') {
            return info + counter ;
        }
    }

    return NULL;
}
/** Function which checks identifiers for keywords
 *
 *  @param string which will be checked for keyword
 *  @return state indicating type of keyword
 ***/
/*tokenType checkForKeyword ( char* string )
{
    //list of all keywords , DON'T change order of elements otherwise you get wrong token types
    const char * kWords [] = { "" , "else" , "float" , "function" , "if" ,  "int" , "null"
                                  ,"return" , "string"  , "void" , "while"  };

    const char * dTypes [] = { "float" , "int" , "string" } ;

    for ( unsigned int counter = 0 ; counter < NUM_OF_KWORDS - 3 ; counter++ )
    {
        if ( ! strcmp ( kWords [ counter ] , string ) ) return counter ; //comparing all keywords and returning which is it
    }
    for( unsigned int counter = 0 ; counter < 3 ; counter++ )
    {
        if ( ! strcmp ( dTypes [ counter] , string )) return identOfType ;
    }

    return identOfFunct ;

}*/ //I will leave it here because it was tested and it works

/** Function which checks identifiers for keywords
 *
 *  @param str which will be checked for keyword
 *  @param token variable where we store type of keyword
 *  @return success of finding keyword
 ***/
bool mapStringToKeyword(const char* str, tokenType* token) {
    //const char * kWords [] = { "" , "else" , "function" , "if" , "null"
      //      ,"return" , "void" , "while"  };

    if (strcmp(str, "") == 0) {
        *token = def;
        return true;
    }

    if (strcmp(str, "else") == 0) {
        *token = elseT;
        return true;
    }

    if (strcmp(str, "function") == 0) {
        *token = functionT;
        return true;
    }

    if (strcmp(str, "if") == 0) {
        *token = ifT;
        return true;
    }

    if (strcmp(str, "null") == 0) {
        *token = nullT;
        return true;
    }

    if (strcmp(str, "return") == 0) {
        *token = returnT;
        return true;
    }

    /*if (strcmp(str, "void") == 0) {
        *token = voidT;
        return true;
    }*/

    if (strcmp(str, "while") == 0) {
        *token = whileT;
        return true;
    }

    return false;
}
/** Function which checks identifiers for data types
 *
 *  @param str which will be checked for data type
 *  @return success of finding data type
 ***/
bool isStringAType(const char* str) {
    //NOLINTNEXTLINE (bugprone-branch-clone)
    if (strcmp(str, "float") == 0) {
        return true;
    } if (strcmp(str, "int") == 0) {
        return true;
    } if (strcmp(str, "string") == 0) {
        return true;
    } if (strcmp(str, "void") == 0) {
        return true;
    }

    return false;
}
/** Function which checks identifiers for keywords
 *
 *  @param string which will be checked for keyword
 *  @return state indicating type of keyword
 ***/
tokenType checkForKeyword ( char* string )
{
    tokenType token;

    if (mapStringToKeyword(string, &token)) {
        return token;
    }

    if (isStringAType(string)) {
        return  identOfType;
    }

    return identOfFunct;
}




/** Function which converts string to number and stores it in token
 *
 *  @param newToken token to which number will be passed
 *  @param info string containing number
 *
 *  @return pointer to token with stored number
 ***/
token_t* convertNum ( token_t* newToken , char* info )
{

    bool decN = false ;

    char * E = strchr( info , 'E' ) ; //checks if E is present in number
    if ( E == NULL )
    {
        E = strchr( info , 'e' ) ;
    }

    decN = findDot( info );

    if ( E != NULL )
    {
        decN = true;
    }
    if ( !decN ){

        newToken->discriminant = integer;

        long num = strtol( info , NULL ,10 );

        if( num > INT_MAX )
        {
            newToken->info.integer = -1;
        }
        else
        {
            newToken->info.integer = (int) num ;
        }
        return newToken;
    }

    newToken->discriminant = decNum;
    newToken->info.decNuber = strtod( info , NULL );
    if( errno == ERANGE) {
        newToken->info.decNuber = -1.0;
    }
    return newToken;
}

/** Function for determinig first state
 *
 *  @param character
 *  @return first state
 ***/
tokenType firstState ( int character )
{
    if ( character == '$') { return identOfVar; }
    if ( isalpha( character ) || character == '_') { return identOfFunct; }
    if ( character == '?' ) { return identOfTypeN;}
    if ( isdigit( character ) ) { return integer; }
    if ( character == '"' ) { return string; }
    if ( character == '/' ) { return division; }
    if ( character == '+' ) { return plusSign; }
    if ( character == '-' ) { return minusSign; }
    if ( character == '*' ) { return multiply; }
    if ( character == '.' ) { return concatenation; }
    if ( character == '<' ) { return lessOper; }
    if ( character == '>' ) { return moreOper; }
    if ( character == '=' ) { return EqOper; }
    if ( character == ':' ) { return colon; }
    if ( character == ';' ) { return semicolon; }
    if ( character == ',' ) { return comma; }
    if ( character == '(' ) { return openParen; }
    if ( character == ')' ) { return closeParen; }
    if ( character == '{' ) { return openSetParen; }
    if ( character == '}' ) { return closeSetParen; }
    if ( character == '!' ) { return notEqOper; }
    return def;

}


/** Function which reads token and sends it to caller
 *
 *
 *  @return pointer to token or error if error of any type occured
 ***/
error(token_ptr) getToken( ) 
{
    if ( interner_ptr == NULL )
    {
        return_error( ERROR_LEX_NOTABLE , token_ptr );
    }
    if( vec_token_ptr_len( &returnedTokens ) != 0)
    {
        error( token_ptr) token = vec_token_ptr_pop_front( &returnedTokens );
        get_value( token_ptr , oldToken , token , token_ptr);
        return_value( oldToken , token_ptr );
    }
    
    bool endState = false;
    bool noRepeat = true ;
    bool dot = true;
    bool exp = true;
    bool sign = true ;
    bool err = false;
    bool escpBSlhs = false;
    static bool firstCall = true;
    int character = 0;
    unsigned int counter = 1 ;
    unsigned int size = DEFAULT_SIZE;
    tokenType state = def;
    error( intern_id ) id;

    token_t* newToken = ( token_t* ) malloc( sizeof( token_t ) ) ;
    char* info = ( char* ) calloc ( DEFAULT_SIZE , sizeof( char ) );

    if( newToken == NULL || info == NULL)
    {
        free( newToken );
        free( info );
        return_error(ERROR_MAL,token_ptr);
    }

    if ( firstCall == true )
    {
        firstCall = false;
        free( info );
        bool correct = checkProlog();
        if ( correct == false )
        {
            free(newToken);
            return_error( ERROR_LEX_PROLOG , token_ptr );
        }
        newToken->info.integer = correct;
        newToken->discriminant = prolog;
        return_value( newToken , token_ptr );
    }
    // discards whitespace characters
    while( isspace( character = getc( stdin ) ) && character != EOF ) {}

    //if EOF is encountered special token is sent
    if( character == EOF )
    {
        free(info);
        ungetc( character , stdin );
        newToken->discriminant = endOfFile;
        newToken->info.integer = -1 ;

        return_value(newToken,token_ptr);
    }


    newToken->discriminant = state = firstState( character );

    //invalid character was found
    if ( state == def ){

        free( info );
        free( newToken );
        return_error(ERROR_LEX,token_ptr);
    }

    info[0] = (char)character ;

    while( true )
    {
        character = fgetc(stdin);

        switch ( state )
        {
            case identOfVar :
                            if ( counter == 1 && ( isalpha( character ) || character == '_'))
                            {
                                //control if following character isn't number or other invalid character
                                info [ counter ] = (char)character;
                                endState = true;
                                counter++;
                                break;
                            }
                            else if ( counter > 1 && ( isalnum( character ) || character == '_') )
                            {
                                info [ counter ] = (char)character ;
                                counter++;
                                break;
                            }

                            if ( endState )
                            {
                                if ( !finalResize( &info , counter) )
                                {
                                    free( info );
                                    free( newToken );
                                    return_error(ERROR_MAL,token_ptr);
                                }
                                ungetc ( character , stdin ); //in case if following character wasn't whitespace

                                __interning();
                                return_value( newToken , token_ptr );
                            }
                            ungetc(character , stdin );
                            free(info);
                            free(newToken);
                            return_error( ERROR_LEX_VAR , token_ptr);

            case identOfFunct :
                            //endState = true ;

                            if ( isalnum ( character ) || character == '_' )
                            {
                                info [ counter ] = (char)character;
                                counter++ ;
                                break ;
                            }

                            ungetc ( character , stdin ) ; // in case if following character wasn't whitespace

                            if ( !finalResize( &info , counter ) )
                            {
                                free (info ) ;
                                free( newToken ) ;
                                return_error( ERROR_MAL , token_ptr );
                            }
                            newToken->discriminant = checkForKeyword ( info );

                            __interning();
                            return_value( newToken , token_ptr );

            case identOfTypeN :
                            if ( counter == 1 && noRepeat )
                            {
                                counter-- ;
                                noRepeat = false ;

                                if ( character == '>' ){
                                    state = endOfFile;
                                    break;
                                }
                            }
                            if ( islower ( character ) ) //after '?' I know there can't upper case character
                            {                            // so I send it down right away to be returned as error
                                info [ counter ] = (char)character ;
                                counter++ ;
                                break ;
                            }

                            ungetc ( character , stdin );
                            if (  !finalResize( &info ,counter) )
                            {
                                free( info ) ;
                                free( newToken ) ;
                                return_error( ERROR_MAL , token_ptr);
                            }
                            if (checkForKeyword ( info ) != identOfType) // if string isn't identificator of type it is error
                            {
                                free( info );
                                free( newToken );
                                return_error(ERROR_LEX_DTYPE, token_ptr );
                            }
                            __interning();
                            return_value( newToken , token_ptr );

            case integer :

                            if ( isdigit( character ) )
                            {
                                if(! exp ) { sign = false; } //if there was 'e' and it was followed by num sign can't appear
                                err = false;             //this marks that 'e' or '.' was followed by number

                                info [ counter ] = (char)character ;
                                counter++ ;
                                break ;
                            }

                            if ( character == '.' && dot)
                            {
                                dot = false;              //this marks that '.' appeared in number
                                err = true;               //following character must be number otherwise error state
                                info [ counter ] = (char)character ;
                                counter++ ;
                                break;
                            }
                            if ( ( character == 'e' || character == 'E' ) && exp && !err )
                            {
                                exp = false; //this marks that 'e' appeared in number
                                dot = false; //this further marks that '.' won't be part of number
                                err = true;  //following character can only be number or sign

                                info [ counter ] = (char)character ;
                                counter++ ;
                                break ;
                            }
                            if ( ( character == '+' || character == '-' ) && sign && !exp)
                            {
                                sign = false; //this marks that sign appeared in number
                                info [ counter ] = (char)character ;
                                counter++ ;

                                break;
                            }
                            if((! isdigit(character) ) && err == true )
                            {
                                free( info );
                                free( newToken );
                                return_error( ERROR_LEX , token_ptr );
                            }

                            ungetc( character , stdin );

                            newToken = convertNum( newToken , info );

                            free ( info );
                            if( newToken->discriminant == decNum )
                            {
                                if(newToken->info.decNuber == -1.0) { return_error( ERROR_LEX,token_ptr); }
                            }
                            else if (newToken->discriminant == integer )
                            {
                                if ( newToken->info.integer == -1 ) { return_error( ERROR_LEX , token_ptr ); }
                            }

                            return_value( newToken , token_ptr );

            case string :
                            if( escpBSlhs == true ) //in escape sequence we want to read almost everything without consequence
                            {
                                if( character == EOF  || character < ' ')
                                {
                                    free ( info ) ;
                                    free ( newToken );
                                    return_error( ERROR_LEX_INVSTR , token_ptr );
                                }
                                info[counter] = (char)character;
                                counter++;
                                escpBSlhs = false;
                                break;
                            }
                            if ( ( character == '$' && info [ counter - 1 ] != '\\' ) || character < ' ' || character == EOF)
                            {
                                // '$' can only be in escape sequence
                                free ( info ) ;
                                free ( newToken );
                                return_error( ERROR_LEX_INVSTR , token_ptr );
                            }

                            if ( character == '"' )
                            {
                                info [ counter ] = (char)character;
                                counter++;
                                if( ! finalResize( &info ,counter) )
                                {
                                    free ( info );
                                    free ( newToken );

                                    return_error( ERROR_MAL , token_ptr );
                                }
                                __interning();
                                return_value( newToken , token_ptr ) ;
                            }

                            if( character == '\\' )
                            {
                                escpBSlhs = true;
                            }
                            info [ counter ] = (char)character;

                            counter++;
                            break;

            case division :
                            if( character != '/' && character != '*' )
                            {
                                ungetc( character , stdin ) ;

                                if( ! finalResize( &info ,counter) )
                                {
                                    free( info );
                                    free ( newToken );
                                    return_error( ERROR_MAL , token_ptr );
                                }
                                newToken->discriminant = division;
                                __interning();
                                return_value( newToken , token_ptr );
                            }
                            else if( character == '/' )
                            {
                                state = lineComment ;
                                break;
                            }
                            else if( character == '*' )
                            {
                                state = multiLineComm;
                                break;
                            }
                            break;
            case lineComment :
                            if( character == '\n' || character == EOF )
                            {
                                free( info );
                                free( newToken );

                                return getToken();
                            }
                            break;
            case multiLineComm:
                            if( character == EOF ) //unterminated ml comment
                            {
                                free( info );
                                free( newToken);
                                return_error( ERROR_LEX_EOFCOMM , token_ptr );
                            }
                            if ( character == '*' )
                            {
                                state = multiLineCommPE;
                            }
                            break;
            case multiLineCommPE :
                            if(character == '/' )
                            {
                                free ( info ) ;
                                free ( newToken ) ; // free everything and call recursively getToken to get token

                                return getToken();

                            }
                            else if( character == EOF ) //unterminated ml comment
                            {
                                free( info );
                                free( newToken);
                                return_error(ERROR_LEX_EOFCOMM , token_ptr );
                            }
                            if( character == '*' ) //there is still chance that next character will be '/'
                            {
                                break;
                            }
                            state = multiLineComm;
                            break;

            case plusSign :   //they all work same so they will fall
            case minusSign :
            case multiply :
            case colon :
            case semicolon :
            case comma :
            case openParen :
            case openSetParen :
            case closeParen :
            case closeSetParen :
            case concatenation :
                            ungetc( character , stdin ) ;

                            if ( ! finalResize( &info ,counter) )
                            {
                                free( info );
                                free( newToken );
                                return_error( ERROR_MAL , token_ptr );
                            }
                            __interning();
                            return_value( newToken , token_ptr );
            case lessOper :
                            if ( character == '=' ){

                                newToken->discriminant = lessOrEqOper ;
                                info [ counter ] = (char)character;
                                counter++;
                            }
                            else
                            {
                                ungetc( character , stdin );
                            }
                            if( ! finalResize( &info ,counter) )
                            {
                                free( info ) ;
                                free( newToken ) ;
                                return_error( ERROR_MAL , token_ptr );
                            }
                            __interning();
                            return_value( newToken , token_ptr );

            case moreOper :
                            if ( character == '=' ){

                                newToken->discriminant = moreOrEqOper ;
                                info [ counter ] = (char)character;
                                counter++;

                            }
                            else
                            {
                                ungetc( character , stdin );
                            }
                            if( ! finalResize( &info ,counter) )
                            {
                                free( info ) ;
                                free( newToken ) ;
                                return_error( ERROR_MAL , token_ptr );
                            }

                            __interning();
                            return_value( newToken , token_ptr ) ;
            case EqOper :
                        if ( counter == 1 && character != '=' )
                        {
                            ungetc( character , stdin ) ;

                            newToken->discriminant = assigment ;

                            if ( ! finalResize( &info ,counter) )
                            {
                                free( info );
                                free( newToken );

                                return_error( ERROR_MAL , token_ptr );
                            }
                            __interning();

                            return_value( newToken , token_ptr );
                        }
                        else if ( counter == 1 && character == '=' )
                        {
                            info [ counter ] = (char)character ;
                            counter++ ;
                            break;
                        }
                        if ( counter == 2 && character == '=' )
                        {
                            info [ counter ] = (char)character ;
                            counter++;

                            if ( ! finalResize ( &info ,counter) )
                            {
                                free( info );
                                free( newToken );
                                return_error( ERROR_MAL , token_ptr );
                            }

                            __interning();

                            return_value( newToken , token_ptr );
                        }

                        free( info );
                        free( newToken );

                        return_error( ERROR_LEX , token_ptr );


            case notEqOper:
                            if ( counter == 1 && character == '=' )
                            {
                                info [ counter ] = (char)character ;
                                counter++;
                                break;
                            }
                            if ( counter == 2 && character == '=' )
                            {
                                info [ counter ] = (char)character ;
                                counter++;
                                if ( ! finalResize( &info ,counter))
                                {
                                    free( info );
                                    free( newToken );
                                    return_error( ERROR_MAL , token_ptr );
                                }
                                __interning();

                                return_value( newToken , token_ptr );
                            }

                            free( info );
                            free( newToken );

                            return_error( ERROR_LEX , token_ptr );
            case endOfFile :
                            free ( info ) ;
                            if ( character == EOF )
                            {
                                newToken->info.integer = -1;
                                newToken->discriminant = endOfFile;
                                return_value( newToken , token_ptr );
                            }
                            free ( newToken );

                            return_error( ERROR_LEX , token_ptr);

            default: break;
        }

        if ( counter >= size )
        {
            if( !resizeString ( &info , &size ) ) //resize string if needed
            {
                free(info);
                free(newToken);
                return_error( ERROR_MAL , token_ptr );
            }
        }

    }

}

/** Function which puts token into vector from which getToken will read it 
 *
 *  @param retToken token which will be returned
 *  @return error if  returning was unsuccessfull
 ***/
error(none) returnToken( token_ptr retToken)
{
    if( retToken == NULL)
    {
        return_error( ERROR_LEX_NOTABLE , none);
    }
    error(none) success = vec_token_ptr_push_front( &returnedTokens , retToken );

    if( is_error(success))
    {
        free( retToken );
        forward_error( success , none );
    }
    return_none();
}
/** Function which skips whitespace characters and compares first different character
 *
 *  @param cmp character for comparison
 *  @return character if comparison was succesfull otherwise 0
 ***/
int skipWhiteSpaceAndCmpChar ( int cmp )
{
    int character = 0;
    tokenType state = def;
    while ( (character = getc ( stdin )) != EOF /*&& isspace( character )*/) {

        switch( state )
        {
            case def:
                    if( character == cmp)
                    {
                        return character;
                    }
                    else if( character == '/')
                    {
                        state = division;
                        break;
                    }
                    else if( isspace( character ))
                    {
                        break;
                    }
                    else
                    {
                        return 0;
                    }
            case division:
                    if( character == '/')
                    {
                        state = lineComment;
                        break;
                    }
                    if( character == '*')
                    {
                        state = multiLineComm;
                        break;
                    }
                    else
                    {
                        return 0;
                    }
            case lineComment:
                    if( character == '\n')
                    {
                        state = def;
                        break;
                    }
                    break;
            case multiLineComm:
                    if( character == '*')
                    {
                        state = multiLineCommPE;
                        break;
                    }
                    break;
            case multiLineCommPE:
                    if( character == '*')
                    {
                        state = multiLineCommPE;
                        break;
                    }
                    else if( character == '/')
                    {
                        state = def;
                        break;
                    }
                    break;
            default:
                    return 0;
        }

    }

    if ( character != cmp ) { return 0; }

    return character ;
}

/** Function which checks if prolog is correct
 *
 *  @return false if prolog is incorect , true if prolog is correct
 ***/
bool checkProlog ()
{
    int character = 0;
    int counter = 0;
    char string [ LEN_OF_PROLSYM + 1 ] = "";
    char string2 [ LEN_OF_DECLARE + 1 ] = "";
    char string3 [ LEN_OF_STRICT + 1 ] = "";
    bool endLine = false;
    bool endLineComm = false;

    tokenType state = plusSign ;

    for( unsigned int counter = 0 ; counter < LEN_OF_PROLSYM - 1 ; counter ++)
    {
        character = getc( stdin ) ;
        string [ counter ] = (char)character ;
    }

    string[LEN_OF_PROLSYM] = '\0';

    if ( strncmp ( string , "<?php", LEN_OF_PROLSYM ) != 0) { return false; }

    while( ( character = getc( stdin ) ) != EOF )
    {

        switch( state )
        {
            case plusSign:
                        if ( character == '/')
                        {
                            state = lineComment;
                        }
                        else if( character == '\n')
                        {
                            endLine = true ;
                            break;
                        }
                        else if( ! isspace( character ) )
                        {
                            return 0;
                        }
                        break;
            case lineComment:
                        if ( character == '/' )
                        {
                            state = identOfFunct;
                            break;
                        }
                        else if ( character == '*' )
                        {
                            state = multiLineComm ;
                            break ;
                        }
                        return 0;
            case identOfFunct:
                        if ( character == '\n' )
                        {

                            endLine = true;
                            break;
                        }
                        break;
            case multiLineComm :
                        if ( character == '\n' )
                        {
                            endLineComm = true ;
                            break;
                        }
                        if ( character == '*' )
                        {
                            state = multiLineCommPE;
                            break;
                        }
                        break;
            case multiLineCommPE :
                        if ( character == '/' && endLineComm == true )
                        {
                            endLine = true ;
                            break;
                        }
                        if( character == '/' && endLineComm == false )
                        {
                            state = plusSign ;
                            break;
                        }
                        state = lineComment ;
                        break ;
            default: return 0;

        }

        if( endLine )
        {

            break;
        }

    }
    if( character == EOF && endLine == false )
    {
        return false;
    }
    counter = 1 ;
    
    string2 [ 0 ] = (char) skipWhiteSpaceAndCmpChar('d') ;
    if( string2 [0] == 0)
    {
        return false;
    }

    while ( ( character = getc ( stdin ))  != EOF )
    {
        string2 [ counter ] = (char)character ;
        counter++ ;
        if( counter == LEN_OF_DECLARE )
        {
            break;
        }
    }

    string2[counter+1] = '\0';

    if ( strncmp( string2 , "declare", counter+1 ) != 0 ) { return false ; }

    if ( ! skipWhiteSpaceAndCmpChar( '(' ) ) { return false; }

    if ( (character = skipWhiteSpaceAndCmpChar( 's' )) == 0) { return false; }


    counter = 1 ;
    string3 [ 0 ] = (char)character ;

     while ( ( character = getc ( stdin ))  != EOF )
    {
        string3 [ counter ] = (char)character ;
        counter++ ;
        if ( counter == LEN_OF_STRICT )
        {
            break;
        }
    }

    string3[counter+1] = '\0';

    if ( strncmp( string3 , "strict_types", counter+1 ) != 0 ) { return false ; }

    if ( ! skipWhiteSpaceAndCmpChar( '=') ) { return false ; }

    if ( ! skipWhiteSpaceAndCmpChar( '1' ) ) { return false; }

    if ( ! skipWhiteSpaceAndCmpChar( ')' )) { return false; }

    if ( ! skipWhiteSpaceAndCmpChar( ';' )) { return false; }

    return true;

}