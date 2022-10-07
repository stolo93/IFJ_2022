#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
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

tokenType checkForKeyword ( char* string ){

    const char * kWords [] = { "else" , "function" , "if"  , "null" , "return" , "void" , " while" };
    const char * dTypes [] = { "float" ,  "int" , "string" } ; 

    for ( unsigned int counter = 0 ; counter < NUM_OF_KWORDS ; counter++ )
    {
        if ( ! strcmp ( kWords [ counter ] , string ) ) return keyword;
    }
    for ( unsigned int counter = 0 ; counter < NUM_OF_DTYPES ; counter++ )
    {
        if ( ! strcmp ( dTypes [ counter ] , string ) ) return identOfType ;
    }

    return identifier ;

}
token* convertNum ( token* newToken , char* info , unsigned int lenght )
{ 
    
    bool decN = false ;
    
    char * E = strchr( info , 'E' ) ;
    if ( E == NULL )
    {
        E = strchr( info , 'e' ) ;
    }
    

    if (  (strchr ( info , '.' )) != NULL ){
        decN == true ;
    }
    if ( E != NULL )
    {
        if ( E [ 1 ] == '-' )
        {
            decN = true;
        }
    }

    if ( !decN ){

        
        newToken->discriminant = integer;
        newToken->info.integer = atoi ( info ) * pow(10 ,  atoi( E + 1));

        
        return newToken;
    }
    else if ( decN )
    {
        newToken->discriminant = decNum;
        newToken->info.decNuber = atof( info );
        return newToken;
    }
    
}


tokenType firstState ( int character )
{
    if ( character == '$') return identOfVar;
    else if ( isalpha( character ) || character == '_') return identifier;
    else if ( character == '?' ) return identOfTypeN;
    else if ( isdigit( character ) ) return integer;
    else if ( character == '"' ) return string;
    else if ( character == '/' ) return devide;
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



token* getToken()
{
    bool endState = false;
    bool noRepeat = true ;
    bool dot = true;
    bool exp = true;
    bool sign = true ; 
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
        free(info);
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
        character = fgetc(stdin);

        //NOTTODO check if char is EOF and send current token away if possible

        switch ( state )
        {
            case identOfVar :
                            if ( counter == 1 && ( isalpha( character ) || character == '_'))
                            {
                                info [ counter ] = character;
                                endState = true;   // TODO check if it should be here
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
                                if ( !finalResize( &info ) )
                                {
                                    free( info );
                                    free( newToken );
                                    return NULL; //TODO that error thing
                                }
                                ungetc ( character , stdin );
                                newToken->info.string = info;

                                return newToken;
                            }
                            ungetc(character , stdin );
                            free(info);
                            free(newToken);
                            return NULL ; // TODO that error thing 

            case identifier :
                            endState = true ;

                            if ( isalnum ( character ) || character == '_' )
                            {
                                info [ counter ] = character;    
                                counter++ ;
                                break ;
                            }

                            ungetc ( character , stdin ) ;

                            if ( !finalResize( &info ) )
                            {
                                free (info ) ;
                                free( newToken ) ;
                                return NULL ; //TODO that error thing
                            }

                            newToken->discriminant = checkForKeyword ( info );
                            newToken->info.string = info ;

                            return newToken;

            case identOfTypeN :
                            if ( counter == 1 && noRepeat )
                            {
                                counter-- ;
                                noRepeat = false ;
                                if ( character == '>') 
                                {
                                    state == endOfFile;
                                }
                            }
                            if ( islower ( character ) )
                            {
                                info [ counter ] = character ;
                                counter++ ;
                                break ;
                            }

                            ungetc ( character , stdin );
                            if (  !finalResize( &info ) || (checkForKeyword ( info ) != identOfType) )
                            {
                                free( info ) ; 
                                free( newToken ) ;
                                return NULL ; //TODO that error thing 
                            }

                            newToken->info.string = info ;

                            return newToken ; 

            case integer :
                            
                            if ( isdigit( character ) )
                            {
                                info [ counter ] = character ;
                                counter++ ;
                                break ;
                            }
                            if ( character == '.' && dot)
                            {
                                dot = false;
                                info [ counter ] = character ;
                                counter++ ;
                                break; 
                            }
                            else if ( ( character == 'e' || character == 'E' ) && exp )
                            {
                                exp = false;
                                info [ counter ] = character ;
                                counter++ ;
                                break ;
                            }
                            else if ( ( character == '+' || character == '-' ) && sign )
                            {
                                sign = false;
                                info [ counter ] = character ;
                                counter++ ;

                                break;  
                            }
                            if ( isalpha( character ))
                            {
                                free( info );
                                free( newToken );
                                return NULL; // plus that error thing
                            }
                            ungetc( character , stdin );

                            newToken = convertNum( newToken , info , counter );
                            free ( info );

                            return newToken ;
                          
            case decNum:
            case string : 
                            if ( character == '$' || character < ' ' )
                            {
                                free ( info ) ;
                                free ( newToken );
                                return NULL ; 
                            }

                            if ( character == '"' )
                            {
                                info [ counter ] = character;
                                if( ! finalResize( &info ) )
                                {
                                    free ( info );
                                    free ( newToken );

                                    return NULL;
                                }

                                newToken->info.string = info ;
                                return newToken;
                            }

                            info [ counter ] = character;

                            counter++;
                            break;

            case devide :
                            if( character != '/' && character != '*' )
                            {
                                ungetc( character , stdin ) ;
                                
                                if( ! finalResize( &info ) )
                                {
                                    free( info );
                                    free ( newToken );
                                    return NULL ;
                                }
                                newToken->discriminant = devide;
                                newToken->info.string = info;

                                return newToken;
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
            case lineComment :
                            if( character == '\n' )
                            {
                                free( info );
                                free( newToken );

                                return getToken();
                            }   
                            break; 
            case multiLineComm:
                            if( character == EOF )
                            {
                                free( info );
                                free( newToken);
                                return NULL;
                            }
                            //continue here         
            case plusSign :
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
                            
                            if ( ! finalResize( &info ) )
                            {
                                free( info );
                                free( newToken );
                                return NULL;
                            }
                            newToken->info.string = info;

                            return newToken;
            case lessOper :
                            if ( character == '=' ){

                                newToken->discriminant = lessOrEqOper ;
                                info [ counter ] = character;

                            }
                            else
                            {
                                ungetc( character , stdin );
                            } 
                            if( ! finalResize( &info ) )
                            {
                                free( info ) ;
                                free( newToken ) ;
                                return NULL;
                            }

                            newToken->info.string = info ;
                            return newToken;
        
            case moreOper :
                            if ( character == '=' ){

                                newToken->discriminant = moreOrEqOper ;
                                info [ counter ] = character;

                            }
                            else
                            {
                                ungetc( character , stdin );
                            } 
                            if( ! finalResize( &info ) )
                            {
                                free( info ) ;
                                free( newToken ) ;
                                return NULL;
                            }

                            newToken->info.string = info ;
                            return newToken;
            case EqOper :
                        if ( counter == 1 && character != '=' )
                        {
                            ungetc( character , stdin ) ;

                            newToken->discriminant = assigment ;

                            if ( ! finalResize( &info ) )
                            {
                                free( info );
                                free( newToken );

                                return NULL;
                            }
                            newToken->info.string = info ;

                            return newToken ;
                        }
                        else if ( counter == 1 && character == '=' )
                        {
                            info [ counter ] = character ;
                            counter++ ;
                            break;
                        }
                        if ( counter == 2 && character == '=' )
                        {
                            info [ counter ] = character ;

                            if ( ! finalResize ( &info ) )
                            {
                                free( info );
                                free( newToken );
                                return NULL ;
                            }

                            newToken->info.string = info ;

                            return newToken ; 
                        }
                        
                        free( info );
                        free( newToken );

                        return NULL;
            

            case notEqOper:
            case endOfFile :
                            free ( info ) ;
                            if ( character == EOF )
                            {
                                newToken->info.integer = -1;

                                return newToken;
                            }
                            free ( newToken );

                            return NULL; //TODO that error thing 

            default: break;
        }

        if ( counter == size )
        {
            if( !resizeString ( &info , &size ) )
            {
                free(info);
                free(newToken);
                return NULL; //TODO that erro thing 
            }
        }

    }

}
