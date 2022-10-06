
#define DEFAULT_SIZE 20

typedef struct token
{

    unsigned int discriminant;
    union information 
    {
        int integer;
        double decNuber;
        char* string;
    };

}token;

token* getToken();

void returnToken(token* retToken);

bool checkProlog();