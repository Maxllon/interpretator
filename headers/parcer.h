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

struct Expretion;

struct Seque;
struct Func;
struct Call;
struct IF;
struct Number;
struct String;
struct Boolean;
struct Variable;
struct Assign;
struct Array;


void* create_expr(expr_kind);
void delete_expr(void*);

struct Seque* parce(tk_node*);

void* parce_expr(tk_node*);

#endif