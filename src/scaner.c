/****************************************************************
 * @name scanner.c
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
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
#include "headers/htab.h"
#include "headers/interner.h"

extern interner* interner_ptr;

#define __interning() do{ id = intern( interner_ptr , info );\
                      if(  is_error( id )){\
                        forward_error( id , token_ptr );}\
                      newToken->info.string = id._value;\
                      }while ( 0 )\
/** Function for resizing strings
 *
 *  @param string which will be resized
 *  @param size current size of string
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
 *  @return pointer pointing at dot in string
 ***/
char * findDot( char* info )
{
    for(unsigned int counter = 0 ; counter < strlen( info ) ; counter++ )
    {
        if( info [ counter ] == '.') return info + counter ;
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
    } else if (strcmp(str, "else") == 0) {
        *token = elseT;
        return true;
    } else if (strcmp(str, "function") == 0) {
        *token = functionT;
        return true;
    } else if (strcmp(str, "if") == 0) {
        *token = ifT;
        return true;
    } else if (strcmp(str, "null") == 0) {
        *token = nullT;
        return true;
    } else if (strcmp(str, "return") == 0) {
        *token = returnT;
        return true;
    } else if (strcmp(str, "void") == 0) {
        *token = voidT;
        return true;
    } else if (strcmp(str, "while") == 0) {
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
    } else if (strcmp(str, "int") == 0) {
        return true;
    } else if (strcmp(str, "string") == 0) {
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
 *  @return token with stored number
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
    if( errno == ERANGE) newToken->info.decNuber = -1.0;
    return newToken;
}

/** Function for determinig first state 
 *
 *  @param character 
 *  @return first state
 ***/
tokenType firstState ( int character )
{
    if ( character == '$') return identOfVar;
    else if ( isalpha( character ) || character == '_') return identOfFunct;
    else if ( character == '?' ) return identOfTypeN;
    else if ( isdigit( character ) ) return integer;
    else if ( character == '"' ) return string;
    else if ( character == '/' ) return division;
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
    else if ( character == '!' ) return notEqOper ;
    else return def;

}


/** Function which reads token and sends it to caller
 *
 *  
 *  @return token or NULL pionter if error occured
 ***/
error(token_ptr) getToken( ) //htab_pair_t_ptr table
{
    if ( interner_ptr == NULL )
    {
        return_error( ERROR_LEX_NOTABLE , token_ptr );
    }
    bool endState = false;
    bool noRepeat = true ;
    bool dot = true;
    bool exp = true;
    bool sign = true ;
    bool err = false;
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
        int correct = checkProlog();
        if ( correct == 0 )
        {
            free(newToken);
            return_error( ERROR_LEX_PROLOG , token_ptr );
        }  
        newToken->info.integer = correct;
        newToken->discriminant = prolog;
        return_value( newToken , token_ptr );
    }
    // discards whitespace characters 
    while( isspace( character = getc( stdin ) ) && character != EOF );

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
                                if(! exp ) sign = false; //if there was 'e' and it was followed by num sign can't appear
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
                            else if ( ( character == 'e' || character == 'E' ) && exp && !err )
                            {
                                exp = false; //this marks that 'e' appeared in number
                                dot = false; //this further marks that '.' won't be part of number
                                err = true;  //following character can only be number or sign

                                info [ counter ] = (char)character ;
                                counter++ ;
                                break ;
                            }
                            else if ( ( character == '+' || character == '-' ) && sign && !exp)
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
                                if(newToken->info.decNuber == -1.0) return_error( ERROR_LEX,token_ptr);
                            }
                            else if (newToken->discriminant == integer )
                            {
                                if ( newToken->info.integer == -1 ) return_error( ERROR_LEX , token_ptr );
                            }                          

                            return_value( newToken , token_ptr );

            case string : 
                            if ( ( character == '$' && info [ counter - 1 ] != '\\' ) || character < ' ' )
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
                            if( character == '\n' )
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
                            ungetc( character , stdin );
                            
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

/** Function which returns token to stdin and frees it
 *
 *  @param retToken token which will be returned to stdin
 *  @return success of returning token
 ***/
error(none) returnToken( token_ptr retToken)
{
    if( retToken->discriminant == integer || retToken->discriminant == decNum )
    {
        int len = 0;
        if (retToken->discriminant == decNum )
        { 
            len = snprintf(NULL , 0 , "%lf" , retToken->info.decNuber ); //getting number of digits in number
        }
        else 
        {
            len = snprintf(NULL , 0 , "%d" , retToken->info.integer );
        }   
        char* string = ( char * ) calloc ( len , sizeof( char )) ; // maybe I overshot it a bit  

        if( string == NULL )
        {
            free( retToken );

            return_error(ERROR_MAL,none);
        }
        if( retToken->discriminant == integer )
        {
            sprintf( string , "%d" , retToken->info.integer );                                                              
        }
        else
        {
            sprintf( string , "%lf" , retToken->info.decNuber );
        }
       
        
        size_t counter = strlen( string ) - 1;

        for(; counter != SIZE_MAX; counter-- )
        {
            ungetc( (int) string [ counter ] , stdin );
        }
        free( string );
        free( retToken );
        return_none();
    }
    
    size_t counter = strlen( retToken->info.string ) - 1;

    for( ; counter != SIZE_MAX ; counter-- )
    {
        ungetc( (int) retToken->info.string [ counter ] , stdin );
    } 

    //free( (char*)retToken->info.string );
    free( retToken );
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
    while ( (character = getc ( stdin )) != EOF && isspace( character )) ;

    if ( character != cmp ) return 0;

    return character ;
}

/** Function which checks if prolog is correct
 *
 *  @return 0 if prolog is incorect , 1 if prolog is correct and 
 *  switch is not present, 2 if prolog is correct and switch is present
 ***/
int checkProlog () 
{
    int character = 0;
    int counter = 0;
    char string [ LEN_OF_PROLSYM ] = "";
    char string2 [ LEN_OF_DECLARE ] = "";
    char string3 [ LEN_OF_STRICT ] = "";
    bool endLine = false;
    bool endLineComm = false;
    
    tokenType state = plusSign ;

    for( unsigned int counter = 0 ; counter < LEN_OF_PROLSYM - 1 ; counter ++)
    {
        character = getc( stdin ) ;
        string [ counter ] = (char)character ;
    }

    if ( strcmp ( string , "<?php" ) != 0) return 0;

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
        return 0; 

    }
    while ( (character = getc ( stdin )) != EOF && isspace( character )) ;

    counter = 1 ;
    if ( character != 'd' )
    {
        ungetc( character , stdin );
        return 1;
    }
    string2 [ 0 ] = (char)character ;
    

    while ( ( character = getc ( stdin ))  != EOF )
    {
        string2 [ counter ] = (char)character ;
        counter++ ;
        if( counter == LEN_OF_DECLARE )
        {
            break;
        }
    }

    if ( strcmp( string2 , "declare" ) != 0 ) return 0 ;

    if ( ! skipWhiteSpaceAndCmpChar( '(' ) ) return 0;

    if ( (character = skipWhiteSpaceAndCmpChar( 's' )) == 0) return 0;

    
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
    
    if ( strcmp( string3 , "strict_types" ) != 0 ) return 0 ;

    if ( ! skipWhiteSpaceAndCmpChar( '=') ) return 0 ;

    if ( ! skipWhiteSpaceAndCmpChar( '1' ) ) return 0;

    if ( ! skipWhiteSpaceAndCmpChar( ')' )) return 0;

    if ( ! skipWhiteSpaceAndCmpChar( ';' )) return 0;

     return 2;     

}