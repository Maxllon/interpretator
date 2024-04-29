#include<lexer.h>

typedef struct associative
{
    char* type;
    char* value;
}associative_t;
static const associative_t associative_list[] = 
{
    {"NUMBER", "1234567890"},
    {"LETTER", "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"},
    {"BINARY", "+-=*/%|&!^"},
    {"SPECIAL", "{}[]()#:"},
};

static associative_t* associative_stream;
static associative_t* associative_stream_end;


static char* create_associative(char* file)
{
    associative_stream = malloc(sizeof(associative_t) * strlen(file));
    associative_t* asst = associative_stream;
    char symb;
    while(*file != '\0')
    {
        for(int i = 0; i < sizeof(associative_list) / sizeof(associative_t); i++)
        {
            if(strchr(associative_list[i].value, *file))
            {
                symb = *(strchr(associative_list[i].value, *file));

                (*asst).type = malloc(strlen(associative_list[i].type)+1);
                (*asst).value = malloc(2);

                strcpy((*asst).type,associative_list[i].type);

                *((*asst).value) = symb;
                *((*asst).value + 1) = '\0';

                asst++;
                break;
            }

        }
        file++;
    }
    associative_stream_end = asst;
    return NULL;
}
static void delete_associative()
{
    associative_t* asst = associative_stream;
    for(int i = 0; i < associative_stream_end - associative_stream; i++)
    {
        free((*asst).type);
        free((*asst).value);
        asst++;
    }
    free(associative_stream);
}

static void out_asst()
{
    associative_t* asst = associative_stream;
    for(int i = 0; i < associative_stream_end - associative_stream; i++)
    {
        printf("Type: {%s} Value: {%s}\n", (*asst).type, (*asst).value);
        asst++;
    }
}

int lexing(char* file, uint is_out_file, uint is_out_tklist)
{
    char* message = create_associative(file);
    if(message != NULL)
    {
        printf("%s", message);
        return 1;
    }
    if (is_out_file) out_asst();

    delete_associative();
    return 0;
}

