#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include"includes.h"
#include"parcer.h"
#include"bm_vector.h"
#include"arena.h"

typedef struct Expretion Expretion;

typedef enum
{
    FUNC_OBJ = 0,
    CALL_OBJ,
    IF_OBJ,

    INTEGER_OBJ,
    FLOAT_OBJ,
    STRING_OBJ,
    BOOLEAN_OBJ,

    BINARY_OBJ,
    ARRAY_OBJ,
    INDEX_OBJ,

    VOID_OBJ,
    BREAK_OBJ,
    RETURN_OBJ
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

struct Object
{
    OBJECT_KIND kind;
    union obj
    {
        long long int_t;
        long double float_t;
        wchar* str;

        bm_vector* array;
        Func_Obj* func;
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

/*
все функции интерпретатора, расположенные в порядке их создания
*/
Environment* create_empty_environment(Environment*);

//функции работы с переменными
Object* find_variable(Environment*, wchar*);
Object* copy_object(Object*);
Object* get_object(Object*);
void add_variable(Environment*, Var_Obj*);

Object* interpretate(Expretion*, Arena*);

Object* interpretate_atom(Expretion*);

Object* interpretate_var(Expretion*);
Object* interpretate_num(Expretion*);
Object* interpretate_str(Expretion*);
Object* interpretate_bool(Expretion*);
Object* interpretate_void(void);

Object* interpretate_bin(Expretion*);

Object* interpretate_list(Expretion*);
Object* interpretate_call(Expretion*);
Object* interpretate_func(Expretion*);
Object* interpretate_return(void);
Object* interpretate_while(Expretion*);
Object* interpretate_foreach(Expretion*);
Object* interpretate_break(void);
Object* interpretate_if(Expretion*);



#endif