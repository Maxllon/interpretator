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
}
