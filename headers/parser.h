#ifndef PARSER_H
#define PARSER_H

#include"includes.h"
#include"lexer.h"
#include"bm_vector.h"
#include"arena.h"

typedef enum
{
    VOID_EXPR = 0,

    SEQUE_EXPR,
    CALL_EXPR,
    IF_EXPR,

    VARIABLE_EXPR,

    ARRAY_EXPR,
    BOOLEAN_EXPR,
    STRING_EXPR,
    NUMBER_EXPR,
    INDEX_EXPR,

    BINARY_EXPR,
    UNARY_EXPR,

    WHILE_EXPR,
    FOREACH_EXPR,

    FUNC_EXPR,
    RETURN_EXPR,

    INSTRUCTION_EXPR
}expr_kind;

typedef struct Expression Expression;

typedef struct Seque Seque;
typedef struct Call Call;
typedef struct If If;

typedef struct Variable Variable;

typedef struct Array Array;
typedef struct Boolean Boolean;
typedef struct String String;
typedef struct Number Number;
typedef struct Index Index;

typedef struct Binary Binary;
typedef struct Unary Unary;

typedef struct While While;
typedef struct Foreach Foreach;

typedef struct Func Func;
typedef struct Return Return;

typedef struct Instruction Instruction;

struct Expression
{
    expr_kind kind;
    union expr
    {
        Seque* seque;
        Call* call;
        If* _if;

        Variable* variable;

        Array* array;
        Boolean* boolean;
        String* string;
        Number* number;
        Index* index;

        Binary* binary;
        Unary* unary;

        While* _while;
        Foreach* foreach;

        Func* func;
        Return* _return;

        Instruction* instruction;
    };
};

struct Seque
{
    bm_vector* expressions;
};

struct Call
{
    wchar* name;
    bm_vector* arguments;
};

struct If
{
    Expression* cond;
    Expression* then;
    Expression* _else;
};

struct Variable
{
    wchar* name;
};

struct Array
{
    bm_vector* expressions;
};

struct Boolean
{
    wchar* value;
};

struct String
{
    wchar* value;
};

struct Number
{
    wchar* value;
};

struct Index
{
    Expression* destination;
    Expression* index;
};

struct Binary
{
    wchar* op;
    Expression* left;
    Expression* right;
};

struct Unary
{
    wchar* op;
    Expression* value;
};

struct While
{
    Expression* cond;
    Expression* body;
};

struct Foreach
{
    Expression* var;
    Expression* list;
    Expression* body;
};

struct Func
{
    wchar* name;
    bm_vector* arguments;
    Expression* body;
};

struct Return
{
    Expression* value;
};

struct Instruction
{
    wchar* name;
};


//create expr functions
Expression* create_empty_expr(expr_kind kind);

Seque* create_empty_seque();
Call* create_empty_call();
If* create_empty_if();

Variable* create_empty_variable();

Array* create_empty_array();
Boolean* create_empty_boolean();
String* create_empty_string();
Number* create_empty_number();
Index* create_empty_index();

Binary* create_empty_binary();
Unary* create_empty_unary();

While* create_empty_while();
Foreach* create_empty_foreach();

Func* create_empty_func();
Return* create_empty_return();

Instruction* create_empty_instruction();


typedef struct op_priority op_priority;
struct op_priority
{
    wchar* operator;
    size_t priority;

};
size_t find_priority(const wchar* op);

//parse functions
Expression* parse(tk_node* main, Arena* arena);

Expression* parse_expr();
Expression* parse_atom();
Expression* mb_binary(Expression* expr, size_t prior);
Expression* mb_index(Expression* expr);

Expression* parse_seque();
Expression* parse_call();
Expression* parse_if();


Expression* parse_array();
Expression* parse_boolean();

Expression* parse_unary();

Expression* parse_while();
Expression* parse_foreach();

Expression* parse_func();
Expression* parse_return();

Expression* parse_instruction();

//output functions

void out_expression(Expression* expr, size_t indent);

void out_void(size_t indent);

void out_seque(Seque* seque, size_t indent);
void out_call(Call* call, size_t indent);
void out_if(If* _if, size_t indent);

void out_variable(Variable* var, size_t indent);

void out_array(Array* array, size_t indent);
void out_boolean(Boolean* boolean, size_t indent);
void out_string(String* string, size_t indent);
void out_number(Number* number, size_t indent);
void out_index(Index* index, size_t indent);

void out_binary(Binary* binary, size_t indent);
void out_unary(Unary* unary, size_t indent);

void out_while(While* _while, size_t indent);
void out_foreach(Foreach* foreach, size_t indent);

void out_func(Func* func, size_t indent);
void out_return(Return* _return, size_t indent);

void out_instruction(Instruction* instruction, size_t indent);

#endif