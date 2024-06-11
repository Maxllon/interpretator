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
    BINARY_EXPR,
    ARRAY_EXPR,
    INDEX_EXPR,
    RETURN_EXPR,
    VOID_EXPR

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
struct Binary;
struct Index;
struct Return;

//expretion vector
typedef struct
{   
    struct Expretion** expr;
    size_t size;
    size_t len;
}expretion_Vector;
struct Expretion* at(expretion_Vector*, size_t);
void push_back(expretion_Vector*, struct Expretion*);

struct Expretion
{
    expr_kind kind;
    union expr
    {
        struct Seque *seque;
        struct Func *func;
        struct Call *call;
        struct IF *If;
        struct Number *number;
        struct String *string;
        struct Boolean *boolean;
        struct Variable *variable;
        struct Assign *assign;
        struct Array *array;
        struct Binary *binary;
        struct Index* index;
        struct Return* return_t;
    };
};
struct Seque
{
    expretion_Vector expretions;
};

struct Func
{
    wchar *name;

    expretion_Vector arguments;

    struct Expretion *body;
};

struct Call
{
    wchar *name;

    expretion_Vector arguments;
};

struct IF
{
    struct Expretion *cond;
    struct Expretion *then;
    struct Expretion *els;
};

struct Number
{
    wchar *value;
};

struct String
{
    wchar *value;
};

struct Boolean
{
    wchar *value;
};

struct Variable
{
    wchar *name;
};

struct Assign
{
    struct Expretion *var;
    struct Expretion *right;
};

struct Array
{
    expretion_Vector expretions;
};

struct Index
{
    struct Expretion* destination;
    struct Expretion* index;
};

struct Binary
{
    struct Expretion *right;
    struct Expretion *left;

    wchar *op;
};

struct Return
{
    struct Expretion* value;
};

//create expr functions
struct Expretion* create_empty_expr(expr_kind);
struct Seque* create_empty_seque(void);
struct Func* create_empty_func(void);
struct Call* create_empty_call(void);
struct IF* create_empty_if(void);
struct Number* create_empty_number(void);
struct String* create_empty_string(void);
struct Boolean* create_empty_boolean(void);
struct Variable* create_empty_variable(void);
struct Assign* create_empty_assign(void);
struct Array* create_empty_array(void);
struct Binary* create_empty_binary(void);
struct Index* create_empty_index(void);
struct Return* create_empty_return(void);


//delete expretion functions
void delete_expr(struct Expretion*);
void delete_seque(struct Seque*);
void delete_func(struct Func*);
void delete_call(struct Call*);
void delete_if(struct IF*);
void delete_number(struct Number*);
void delete_string(struct String*);
void delete_boolean(struct Boolean*);
void delete_variable(struct Variable*);
void delete_assign(struct Assign*);
void delete_array(struct Array*);
void delete_binary(struct Binary*);
void delete_index(struct Index*);
void delete_return(struct Return*);


typedef struct 
{
    wchar* operator;
    size_t priority;

}op_priority;
size_t find_priority(const wchar*);

//parce functions
struct Expretion* parce(tk_node* main);
struct Expretion* parce_expr(void);
struct Expretion* parce_atom(void);
struct Expretion* mb_binary(struct Expretion*, size_t);
struct Expretion* mb_index(struct Expretion*);
struct Expretion* parce_if(void);
struct Expretion* parce_seque(void);
struct Expretion* parce_func(void);
struct Expretion* parce_call(void);
struct Expretion* parce_return(void);

//output functions
void out_expretion(struct Expretion*, size_t);
void out_seque(struct Seque*, size_t);
void out_binary(struct Binary*, size_t);
void out_number(struct Number*, size_t);
void out_variable(struct Variable*, size_t);
void out_string(struct String*, size_t);
void out_if(struct IF*, size_t);
void out_seque(struct Seque*, size_t);
void out_func(struct Func*, size_t);
void out_call(struct Call*, size_t);
void out_index(struct Index*, size_t);
void out_return(struct Return*, size_t);
void out_void(size_t);
void out_boolean(struct Boolean*, size_t);

#endif