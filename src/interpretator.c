#include"interpretator.h"

Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = malloc(sizeof(Environment));
    envi->parent = parent;
    envi->variables = bm_vector_create();
    return envi;
}

Object* create_empty_object(OBJECT_KIND kind)
{
    Object* obj = malloc(sizeof(Object));
    obj->kind = kind;
    switch(kind)
    {
        case VARIABLE_OBJ:
            obj->var = create_empty_var_obj();
            break;
        case FUNCTION_OBJ:
            obj->func = create_empty_func_obj();
            break;
        case INTEGER_OBJ:
            break;
        case STRING_OBJ:
            obj->str = NULL;
            break;
        case FLOAT_OBJ:
            break;
        case BOOLEAN_OBJ:
            break;
        case LIST_OBJ:
            obj->list = bm_vector_create();
            break;
        case EMPTY_OBJ:
            break;
        default:
            wprintf(L"Ошибка: Неизвестный тип обьекта!!\n");
            system("pause");
            exit(1);
    }
    return obj;
}

Var_Obj* create_empty_var_obj(void)
{
    Var_Obj* var = malloc(sizeof(Var_Obj));
    var->name = NULL;
    var->value = NULL;
    return var;
}

Func_Obj* create_empty_func_obj(void)
{
    Func_Obj* func = malloc(sizeof(Func_Obj));
    func->arguments = bm_vector_create();
    func->expr = NULL;
    return func;
}

void free_obj(Object* obj)
{
    if(obj == NULL) return;
    switch(obj->kind)
    {
        case VARIABLE_OBJ:
            free_var_obj(obj->var);
            break;
        case FUNCTION_OBJ:
            free_func_obj(obj->func);
            break;
        case INTEGER_OBJ:
            break;
        case STRING_OBJ:
            bm_free(obj->str);
            break;
        case FLOAT_OBJ:
            break;
        case BOOLEAN_OBJ:
            break;
        case LIST_OBJ:
            bm_vector_free(obj->list);
            break;
        case EMPTY_OBJ:
            break;
        default:
            wprintf(L"Ошибка: Неизвестный тип обьекта!!\n");
            system("pause");
            exit(1);
    }
    free(obj);
}

void free_environment(Environment* envi)
{
    for(size_t i = 0; i < envi->variables->len; ++i)
    {
        free(bm_vector_at(envi->variables, i));
    }
    free(envi);
}

void free_var_obj(Var_Obj* obj)
{
    if(obj==NULL) return;
    bm_free(obj->name);
    free_obj(obj->value);
    free(obj);
}

void free_func_obj(Func_Obj* obj)
{
    if(obj==NULL) return;
    bm_vector_free(obj->arguments);
    free(obj);
}
