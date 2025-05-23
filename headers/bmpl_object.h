#ifndef BMPL_OBJECT_H
#define BMPL_OBJECT_H

#include"arena.h"
#include"includes.h"
#include"bmpl_string.h"
#include"long_num.h"
#include"parser.h"
#include"dekart_tree.h"

typedef enum
{
    VOID_OBJ = 0,
    NUM_OBJ,
    FUNC_OBJ,
    STR_OBJ,
    BOOL_OBJ,
    LIST_OBJ,
    INSTRYCTION_OBJ,
    RETURN_OBJ

}bmpl_object_types;

typedef enum
{
    BREAK = 0,
    CONTINUE
}instruction_types;

typedef struct bmpl_object bmpl_object;
typedef struct func_object func_object;

struct bmpl_object
{
    bmpl_object_types type;

    union obj
    {
        big_num* num;
        bmpl_string* str;
        func_object* func;
        int _bool;
        dk_node* root;
        instruction_types instr;
        bmpl_object* ret;
    };
};

struct func_object
{
    Expression* body;
    bm_vector* args;
    int is_std;
};

bmpl_object* new_object(bmpl_object_types type, void* value, Arena* arena);
bmpl_object* copy_object(bmpl_object* src, Arena* arena);
bmpl_object* get_object(bmpl_object* src, Arena* arena);

typedef struct module module;
typedef struct variable variable;

struct variable
{
    bmpl_string* name;
    bmpl_object* value;
};

variable* new_variable(bmpl_string* name, bmpl_object* value, Arena* arena);

struct module
{
    dk_node* variables;
    module* parent;
};

module* new_module(Arena* arena, module* parent);
variable* find_var(module* main, bmpl_string* str, Arena* arena);
void add_var(module* main, variable* var, Arena* arena);


#endif