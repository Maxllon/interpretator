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
    ARRAY_EXPR

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

#endif