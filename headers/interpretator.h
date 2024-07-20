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
    LIST_OBJ,
    INDEX_OBJ,

    VOID_OBJ,
    BREAK_OBJ,
    RETURN_OBJ,

    VARIABLE_OBJ
}OBJECT_KIND;

typedef struct Object Object;
typedef struct Environment Environment;

typedef struct Var_Obj Var_Obj;
typedef struct Func_Obj Func_Obj;
typedef struct Index_Obj Index_Obj;

struct Environment
{
    bm_vector* variables;
    Environment* parent;
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

struct Index_Obj
{
    Object* list;
    Object* index;
};

struct Object
{
    OBJECT_KIND kind;
    union obj
    {
        long long int_t;
        long double float_t;
        int bool_t;
        wchar* str;

        bm_vector* list;

        Var_Obj var;
        Func_Obj func;
        Index_Obj index;
    };
    
};


wchar* out_type(Object* obj);

Environment* create_empty_environment(Environment* parent);

Object* find_var(wchar* name, Environment* envi);
void set_var(Object* variable);

//возвращает сам объект или его копию
Object* get_object(Object* obj);


void interpretate(Expretion* expr, Arena* arena);

Object* interpretate_atom(Expretion* expr);


Object* interpretate_var(Expretion* expr);

//типы данных
Object* interpetate_num(Expretion* expr);
Object* interpretate_str(Expretion* expr);
Object* interperate_bool(Expretion* expr);
Object* interpretate_list(Expretion* expr);
Object* interpretate_void(Expretion* expr);

//работа с типами
Object* interpretate_bin(Expretion* expr);
Object* interpretate_index(Expretion* expr);

//ветвления
Object* interpetate_if(Expretion* expr);

//циклы
Object* interpretate_break(Expretion* expr);
Object* interpetate_while(Expretion* expr);
Object* interpretate_foreach(Expretion* expr);

//функции
Object* interpretate_return(Expretion* expr);
Object* interpetate_func(Expretion* expr);
Object* interpretate_call(Expretion* expr);


#endif