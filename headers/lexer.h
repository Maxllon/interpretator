#ifndef LEXER_H
#define LEXER_H

#include"includes.h"

const wchar* KEYWORDS = L"если пока иначе вернуть не брать из функ нч кн";
const wchar* STDFUNC = L"печать ввод промежуток";
const wchar* BIN_OP = L"+ - = == * / // ^ != > < <= >= ост и или";
const wchar* NUMBER = L"0123456789.";
const wchar* SPEC = L";,()\"";

typedef struct
{
    int x;
    int y;
}VEC_2;

typedef enum
{
    END = 0,
    KEYWORD,
    VARIABLE,
    STRING,
    BINARY,
    SPECIAL
}TOKEN_KIND;

typedef struct tk_node_t
{
    TOKEN_KIND kind;
    wchar* value;
    VEC_2 pos;

    struct tk_node_t* next;
    struct tk_node_t* previous;
}tk_node;

void push_node(tk_node* main, tk_node* node);
tk_node* new_node(TOKEN_KIND kind, wchar* value, VEC_2 pos);
void delete_tk_list(tk_node* main);

tk_node* lexing(wchar* file);


#endif