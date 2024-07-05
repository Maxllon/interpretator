#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include"includes.h"
#include"parcer.h"
#include"bm_vector.h"
#include"arena.h"

typedef struct Expretion Expretion;

typedef enum
{
    VARIABLE_OBJ = 0,
    FUNCTION_OBJ,
    INTEGER_OBJ,
    FLOAT_OBJ,
    STRING_OBJ,
    TRUE_OBJECT,
    FALSE_OBJECT,
    LIST_OBJ,
    EMPTY_OBJ
}OBJECT_KIND;

typedef struct Object Object;
typedef struct Environment Environment;

typedef struct Var_Obj Var_Obj;
typedef struct Func_Obj Func_Obj;
typedef struct List_Obj List_Obj;

struct Environment
{
    bm_vector* variables;
    Environment* parent;
};
Object* find_object(Environment* envi, wchar*);
Object* get_object(Object*);
void add_object(Object*);

struct Object
{
    OBJECT_KIND kind;
    union obj
    {
        long long int_t;
        long double float_t;
        wchar* str;

        bm_vector* list;
        Func_Obj* func;
        Var_Obj* var;
    };
    
};

struct Var_Obj
{
    wchar* name;
    Object* value;
};

struct Func_Obj
{
    bm_vector* arguments;
    Expretion* expr;
};

Environment* create_empty_environment(Environment*);

Object* interpretate_var(Expretion*);//
Object* interpretate_bin(Expretion*);
Object* interpretate_num(Expretion*);//
Object* interpretate_atom(Expretion*);
Object* interpretate_str(Expretion*);//
Object* interpretate_bool(Expretion*);//
Object* interpretate_list(Expretion*);//
Object* interpretate_void(void);//
Object* interpretate_call(Expretion*);
Object* interpretate_func(Expretion*);
Object* interpretate_while(Expretion*);
Object* interpretate_foreach(Expretion*);
Object* interpretate_if(Expretion*);


Object* interpretate(Expretion*, Arena*);



#endif