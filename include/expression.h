#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<my_string.h>
#include<token.h>
#include<my_link_list.h>

typedef struct expression expression;

typedef struct num_expr num_expr;

typedef struct un_expr un_expr;
typedef struct bin_expr bin_expr;

typedef struct seque_expr seque_expr;

typedef struct if_expr if_expr;
typedef struct var_expr var_expr;
typedef struct index_expr index_expr;
typedef struct class_expr class_expr;
typedef struct foreach_expr foreach_expr;
typedef struct while_expr while_expr;
typedef struct func_expr func_expr;
typedef struct call_expr call_expr;
typedef struct list_expr list_expr;
typedef struct instryct_expr instryct_expr;
typedef struct return_expr return_expr;
typedef struct string_expr string_expr;
typedef struct method_expr method_expr;
typedef struct bool_expr bool_expr;

typedef enum expr_kind
{
    EXPR_NUM,
    EXPR_UN,
    EXPR_BIN,
    EXPR_SEQUE,
    EXPR_IF,
    EXPR_VAR,
    EXPR_INDEX,
    EXPR_CLASS,
    EXPR_FOREACH,
    EXPR_WHILE,
    EXPR_FUNC,
    EXPR_CALL,
    EXPR_LIST,
    EXPR_INSTRYCT,
    EXPR_RETURN,
    EXPR_STRING,
    EXPR_METHOD,
    EXPR_BOOL
}expr_kind;

struct expression
{
    expr_kind type;
    pos _pos;
    union expr_type
    {
        num_expr* number;
        un_expr* unary;
        bin_expr* binary;
        seque_expr* seque;
        if_expr* _if;
        var_expr* var;
        index_expr* index;
        class_expr* class;
        foreach_expr* _for;
        while_expr* _while;
        func_expr* func;
        call_expr* call;
        list_expr* list;
        instryct_expr* instryct;
        return_expr* _return;
        string_expr* _string;
        method_expr* method;
        bool_expr* _bool;

    }expr;
};

struct bool_expr
{
    bool_t is_true;
    string* value;
};

struct method_expr
{
    expression* dest;
    expression* call;
};

struct string_expr
{
    string* value;
};

struct return_expr
{
    expression* expr;
};

struct instryct_expr
{
    int type;
};

struct list_expr
{
    link_node* list;
};

struct func_expr
{
    expression* call;
    expression* body;
};

struct call_expr
{
    expression* dest;
    link_node* argc;
};

struct while_expr
{
    expression* cond;
    expression* body;
};

struct foreach_expr
{
    expression* iter;
    expression* list;
    expression* body;
};

struct class_expr
{
    expression* name;
    expression* derive;
    expression* body;
};

struct index_expr
{
    expression* index;
    expression* dest;
};

struct var_expr
{
    string* value;
};

struct num_expr
{
    string* value;
};

struct un_expr
{
    string* op;
    op_type type;
    expression* expr;
};

struct bin_expr
{
    string* op;
    op_type type;
    expression* left;
    expression* right;
};

struct seque_expr
{
    link_node* list;
};

struct if_expr
{
    expression* cond;
    expression* then;
    expression* _else;
};

expression* 
parse(link_node* node);


#define BASE_INDENT 4
void
out_AST(expression* root);

#endif