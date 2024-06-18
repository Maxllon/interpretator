#ifndef LEXER_H
#define LEXER_H

#include"includes.h"
#include"arena.h"

typedef struct
{
    int x;
    int y;
} VEC_2;

typedef enum
{
    END = 0,
    START,
    
    KEYWORD,
    VARIABLE,
    STRING,
    BINARY,
    NUMBER,

    SPECIAL,
    DIGIT,
    SYMBOL,
    BIN
}TOKEN_KIND;

typedef struct tk_node tk_node;
struct tk_node
{
    TOKEN_KIND kind;
    wchar *value;
    VEC_2 pos;

    tk_node *next;
    tk_node *previous;
};

void push_node(tk_node*, tk_node*);
tk_node *new_node(TOKEN_KIND, wchar*, VEC_2);
tk_node *go_start(tk_node*);

void out_tk_list(tk_node*);

tk_node *lexing(wchar*, Arena*);

#endif