#include <stdbool.h>
#define DEFAULT_SIZE 20

typedef enum tokenType { def , keyword , identifier , identOfVar , identOfType , 
             Integer , decNum , string , lineComment , multiLineComm ,
             plusSign, minusSign , multiply , devide , concatenation ,
             lessOper , lessOrEqOper , moreOper , moreOrEqOper ,
             EqOper, assigment , openParen , closeParen , openSetParen , 
             closeSetParen , comma , semicolon , colon , endOfFile } tokenType;

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

void returnToken ( token* retToken ) ;

bool checkProlog () ;