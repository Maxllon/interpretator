#include"lexer.h"
tk_node* new_node(TOKEN_KIND kind, wchar* value, VEC_2 pos)
{
    tk_node* node = malloc(sizeof(tk_node));
    node->kind = kind;
    node->next = NULL;
    node->previous = NULL;
    node->pos = pos;
    node->value = malloc(sizeof(wchar) * wcslen(value) + 2);
    wcscpy(node->value, value);
    return node;
}

void delete_tk_list(tk_node* main)
{
    if(main->next != NULL) delete_tk_list(main->next);
    else free(main->next);
    free(main->value);
    free(main);
}

void push_node(tk_node* main, tk_node* node)
{
    tk_node* st = main;
    while(st->next != NULL)
    {
        st = st->next;
    }
    st->next = node;
    node->previous = st;
}

tk_node* go_start(tk_node* main)
{
    while(main->previous != NULL) main = main->previous;
    return main;
}

const wchar* KEYWORDS = L"если пока иначе вернуть не брать из функ нч кн";
const wchar* STDFUNC = L"печать ввод промежуток";
const wchar* BIN_OP = L"+ - = == * / // ^ != > < <= >= ост и или";
const wchar* NUMBER = L"0123456789.";
const wchar* SPEC = L";,()\"";

tk_node* lexing(wchar* file)
{
    tk_node* main = NULL;

    return main;
}