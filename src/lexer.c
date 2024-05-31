#include"lexer.h"
tk_node* new_node(TOKEN_KIND kind, wchar* value, VEC_2 pos)
{
    tk_node* node = malloc(sizeof(tk_node));
    node->kind = kind;
    node->next = NULL;
    node->previous = NULL;
    node->pos = pos;
    node->value = value;
    return node;
}

void delete_tk_list(tk_node* main)
{
    if(main == NULL) return;
    if(main->next != NULL) delete_tk_list(main->next);
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
const wchar* BIN_OP = L"+-=*/^!><";
const wchar* NUMBER = L"0123456789.";
const wchar* SPEC = L";,()\"";

tk_node* lexing(wchar* file)
{
    //--------------------------lexing symbols-------------------------
    VEC_2 pos = {0,0};
    tk_node* symbols = new_node(START, NULL,pos);
    wchar* str;
    wchar sym;
    for(u8 i = 0; i < wcslen(file); ++i)
    {
        sym = *(file+i);
        if(wcschr(SPEC, sym) != NULL)
        {
            str = malloc(sizeof(wchar) * 2);
            *str = sym;
            *(str+1) = L'\0';
            push_node(symbols, new_node(SPECIAL, str, pos));
        }
        else if(sym == L'\n')
        {
            pos.y++;
            pos.x = 0;
        }
        else if(wcschr(BIN_OP, sym) != NULL)
        {
            str = malloc(sizeof(wchar) * 2);
            *str = sym;
            *(str+1) = L'\0';
            push_node(symbols, new_node(BINARY, str, pos));
        }
        else if(wcschr(NUMBER, sym) != NULL)
        {
            str = malloc(sizeof(wchar) * 2);
            *str = sym;
            *(str+1) = L'\0';
            push_node(symbols, new_node(DIGIT, str, pos));
        }
        else
        {
            str = malloc(sizeof(wchar) * 2);
            *str = sym;
            *(str+1) = L'\0';
            push_node(symbols, new_node(SYMBOL, str, pos));          
        }
        if(sym != L'\n') pos.x++;
    }
    push_node(symbols, new_node(END, NULL, pos));
    //----------------------end lexing symbols-------------------------

    tk_node* main = new_node(START, NULL,pos);
    while(symbols->kind != END)
    {
        //lex

        
        symbols = symbols->next;
    }
    symbols = go_start(symbols);
    delete_tk_list(symbols);
    return main;
}