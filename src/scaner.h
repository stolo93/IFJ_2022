/****************************************************************
 * @name scanner.h
 * @author : Jozef Michal Bukas <xbukas00@stud.fit.vutbr.cz>
 * Subject : IFJ
 * Project : Compiler for a given subset of the php language
****************************************************************/
#include <stdbool.h>

#define DEFAULT_SIZE 20
#define NUM_OF_KWORDS 7
#define NUM_OF_DTYPES 3

typedef enum tokenType { def , keyword , identifier , identOfVar , identOfType , 
             integer , decNum , string , lineComment , multiLineComm ,
             plusSign, minusSign , multiply , devide , concatenation ,
             lessOper , lessOrEqOper , moreOper , moreOrEqOper ,
             EqOper , notEqOper, assigment , openParen , closeParen , openSetParen , 
             closeSetParen , comma , semicolon , colon , endOfFile , identOfTypeN ,
             multiLineCommPE } tokenType;

typedef struct token
{

    tokenType discriminant;
    union information 
    {
        int integer;
        double decNuber;
        char* string;
    } info;

} token ;

tokenType firstState( int character );

token* getToken ();

bool returnToken ( token* retToken ) ;

bool checkProlog () ;