#ifndef PARCER_H
#define PARCER_H

#include"includes.h"
#include"lexer.h"

typedef enum
{
    SEQUE_EXPR = 0,
    FUNC_EXPR,
    CALL_EXPR,
    IF_EXPR,

    NUMBER_EXPR,
    STRING_EXPR,
    BOOLEAN_EXPR,
    VARIABLE_EXPR,

    ASSIGN_EXPR,
    BINARY_EXPR

}expr_kind;

struct Seque
{
    expr_kind kind;
    void** expretions;
};

struct Func
{
    expr_kind kind;
    wchar* name;
    void** args;
    struct Seque* body;
};

struct Call
{
    expr_kind kind;
    wchar* name;
    void** args;
};

struct If
{
    expr_kind kind;
    void* condition;
    void* then;
    void* els;
};

struct Number
{
    expr_kind kind;
    wchar* value;
};

struct String
{
    expr_kind kind;
    wchar* value;
};

struct Boolean
{
    expr_kind kind;
    int value;
};

struct Variable
{
    expr_kind kind;
    wchar* name;
};

struct Assign
{
    expr_kind kind;
    struct Variable left;
    void* right;
};

struct Binary
{
    expr_kind kind;
    void* left;
    void* right;
};

void* create_expr(expr_kind);
void delete_expr(void*);

struct Seque* parce(tk_node*);

void* parce_expr(tk_node*);

#endif