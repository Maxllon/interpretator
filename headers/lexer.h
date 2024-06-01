#ifndef LEXER_H
#define LEXER_H

#include "includes.h"

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
    SPECIAL,
    DIGIT,
    PLAIN,
    SYMBOL,
    NUMBER
} TOKEN_KIND;

typedef struct tk_node_t
{
    TOKEN_KIND kind;
    wchar *value;
    VEC_2 pos;

    struct tk_node_t *next;
    struct tk_node_t *previous;
} tk_node;

void push_node(tk_node *main, tk_node *node);
tk_node *new_node(TOKEN_KIND kind, wchar *value, VEC_2 pos);
void delete_tk_list(tk_node *main);
tk_node *go_start(tk_node *main);

void out_tk_list(tk_node *main);

tk_node *lexing(wchar *file);

#endif