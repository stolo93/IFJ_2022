#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"


int main()
{

    token* tek;
    

    while ( true )
    {
        tek = getToken();

        if( tek == NULL){
            printf( " invalid token or other internal error \n");
            continue;
        }
        if( tek->discriminant == endOfFile){
           
            printf(" type : %d  informataion : %d\n", tek->discriminant , tek->info.integer);
            free( tek );
            break;
        }
        if( tek->discriminant == integer){
            printf(" type : %d  informataion : %d\n", tek->discriminant , tek->info.integer);
            free( tek );
            continue;
        }
        if( tek->discriminant == decNum ){
            printf(" type : %d  informataion : %lf\n", tek->discriminant , tek->info.decNuber);
            free( tek );
            continue;
        }
        printf(" type : %d  informataion : %s\n", tek->discriminant , tek->info.string);
        free( tek->info.string);
        free( tek );
        
    }
   
    return 0;
}