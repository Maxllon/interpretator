#ifndef DORL_OBJECT_H
#define DORL_OBJECT_H

#include<my_string.h>
#include<long_num.h>
#include<expression.h>
#include<dekart_tree.h>
#include<token.h>
#include<globals.h>

typedef struct dorl_object dorl_object;
typedef struct environment environment;

typedef enum object_type
{
    OBJ_NUM,
    OBJ_STR,
    OBJ_FUNC,
    OBJ_LIST,
    OBJ_CLASS,
    OBJ_BOOL,
    OBJ_VOID,
    OBJ_RETURN,
    OBJ_INSTRYCTION
}object_type;

typedef struct func_obj func_obj;

struct dorl_object
{
    object_type type;
    union obj
    {
        big_num* num;
        string* str;
        func_obj* func;
        dk_node* root;
        bool_t _bool;
        dorl_object* ret_value;
        instryction_type instr_type;
    }obj;
    environment* methods;
};

struct environment
{
    link_node* var_list;
    environment* parent;
};
struct func_obj
{
    link_node* args;
    size_t cnt;

    bool_t is_dorl;
    union src
    {
        expression* expr;
        dorl_object* (*c_func)(link_node*);
    }src;
};

dorl_object* dorl_object_create(object_type type);
dorl_object* dorl_object_copy(dorl_object* obj);
dorl_object* dorl_object_get(dorl_object* obj);

environment* environment_create(environment* parent);
environment* environment_copy(environment* envi);
dorl_object* environment_get(string* name, environment* dest);
int environment_change(string* name, dorl_object* obj, environment* dest);

extern environment* str_methods;
extern environment* list_methods;

#endif