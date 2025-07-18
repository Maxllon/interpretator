#include<defines.h>
#include<my_string.h>
#include<globals.h>
#include<arena.h>
#include<read_file.h>
#include<token.h>
#include<expression.h>
#include<long_num.h>
#include<interpretator.h>

Arena* ARENA = NULL;
error LAST_ERROR = {0};


int main()
{
    ARENA = arena_create();
    LAST_ERROR.type = ALL_GOOD;

    string* str = str_init(U"");
    read_txt_file(str, "samples/test.dorl");

    str_out(str);

    link_node* token_list = tokenize(str);
    if(LAST_ERROR.type == ALL_GOOD) out_tk_list(token_list);
    else
    {
        printf("Ошибка в лексере: %d\n", LAST_ERROR.type);
        arena_destroy();
        return 0;
    }
    expression* AST = parse(token_list);
    if(LAST_ERROR.type == ALL_GOOD) out_AST(AST);
    else
    {
        printf("Ошибка в парсере: %d\n", LAST_ERROR.type);
        arena_destroy();
        return 0;
    }
    printf("\n\n");
    interpretate(AST);
    printf("\n");
    arena_destroy();
    return 0;
}