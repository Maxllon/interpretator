#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include"includes.h"
#include"arena.h"
#include"parser.h"
#include"bmpl_object.h"

void interpretate(Expression* expr, Arena* arena);

bmpl_object* interpretate_atom(Expression* expr);

/*VOID_EXPR = 0,

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

INSTRUCTION_EXPR*/

bmpl_object* get_object_type(bmpl_object_types type);


bmpl_object* interpretate_void(Expression* expr);

bmpl_object* interpretate_seque(Expression* expr);
bmpl_object* interpretate_call(Expression* expr);
bmpl_object* interpretate_if(Expression* expr);

bmpl_object* interpretate_var(Expression* expr);

bmpl_object* interpretate_array(Expression* expr);
bmpl_object* interpretate_bool(Expression* expr);
bmpl_object* interpretate_str(Expression* expr);
bmpl_object* interpretate_num(Expression* expr);
bmpl_object* interpretate_index(Expression* expr);

bmpl_object* interpretate_binary(Expression* expr);
bmpl_object* interpretate_unary(Expression* expr);

bmpl_object* interpretate_while(Expression* expr);
bmpl_object* interpretate_foreach(Expression* expr);

bmpl_object* interpretate_func(Expression* expr);
bmpl_object* interpretate_return(Expression* expr);

bmpl_object* interpretate_instryction(Expression* expr);

//операторы

bmpl_object* assign(Expression* expr);

bmpl_object* bin_num_num(wchar* op, big_num* left, big_num* right);
bmpl_object* bin_str_str(wchar* op, bmpl_string* left, bmpl_string* right);
bmpl_object* bin_str_num(wchar* op, bmpl_string* left, big_num* right);
bmpl_object* bin_bool_bool(wchar* op, int left, int right);
bmpl_object* bin_list_list(wchar* op, dk_node* left, dk_node* right);
bmpl_object* bin_list_num(wchar* op, bmpl_object* left, big_num* right);

bmpl_object* un_bool(wchar* op, int value);
bmpl_object* un_num(wchar* op, big_num* value);

#endif