#include"interpretator.h"
#include"arena.h"

//глобальные переменные
static Arena *ARENA = NULL;
static Environment* current_envi = NULL;
static Object* empty_object = NULL;

Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = arena_alloc(ARENA, sizeof(Environment));
    envi->parent = parent;
    envi->variables = bm_vector_create(ARENA);
    return envi;
}

Object* find_variable(Environment* envi, wchar_t* name) 
{
    for (int i = 0; i < envi->variables->len; ++i) 
    {
        Var_Obj* var_obj = (Var_Obj*)bm_vector_at(envi->variables, i);
        if (wcscmp(var_obj->name, name) == 0) 
        {
            return var_obj->value;
        }
    }
    if (envi->parent != NULL) 
    {
        return find_variable(envi->parent, name);
    }
    return NULL;
}

// Копирование объекта
Object* copy_object(Object* obj) {
    Object* new_obj = arena_alloc(ARENA, sizeof(Object));
    new_obj->kind = obj->kind;
    
    switch(obj->kind) {
        case INTEGER_OBJ:
            new_obj->int_t = obj->int_t;
            break;
        case FLOAT_OBJ:
            new_obj->float_t = obj->float_t;
            break;
        case STRING_OBJ:
            new_obj->str = arena_alloc(ARENA, sizeof(wchar) * (wcslen(obj->str) + 1));
            wcscpy(new_obj->str, obj->str);
            break;
        default:
            wprintf(L"Не могу копироовать обьект\n");
            EXIT;
        // Добавьте обработку других типов
    }
    
    return new_obj;
}

//решает копировать объект или нет
Object* get_object(Object* obj)
{
    switch(obj->kind)
    {
        case INTEGER_OBJ:
            return copy_object(obj);
        case FLOAT_OBJ:
            return copy_object(obj);
        case STRING_OBJ:
            return copy_object(obj);
        default:
            wprintf(L"Cant get object with this type: %d\n", obj->kind);
            EXIT;

    }
    return NULL;
}

void add_variable(Environment* envi, Var_Obj* var)
{ 
    if(envi == NULL)
    {
        wprintf(L"cant add var\n");
        EXIT;
    }
    bm_vector_push(envi->variables, var);
}

Object* interpretate(Expretion* expr, Arena* arena)
{
    ARENA = arena;

    Environment* envi = create_empty_environment(NULL);
    current_envi = envi;

    empty_object = arena_alloc(ARENA, sizeof(Object));
    empty_object->kind = VOID_OBJ;

    
    for(size_t i = 0; i < expr->seque->expretions->len; ++i)
    {
       interpretate_atom((Expretion*)bm_vector_at(expr->seque->expretions, i));
    }

    return NULL;
}

/*
интерпретирует выражения по одному
*/
Object* interpretate_atom(Expretion* expr)
{
    switch(expr->kind)
    {
        case VARIABLE_EXPR:
            return interpretate_var(expr);
        case NUMBER_EXPR:
            return interpretate_num(expr);
        case STRING_EXPR:
            return interpretate_str(expr);
        case VOID_EXPR:
            return empty_object;
        default:
            wprintf(L"cant interpretate atom!\n");
            EXIT;
    }
}

/*
возвращает NULL если нет переменной или если она не определенна
возвращает ссылку на значение переменной
*/
Object* interpretate_var(Expretion* expr)
{
    Object* obj = find_variable(current_envi, expr->variable->name);
    if(obj == NULL)
    {
        Var_Obj* var = arena_alloc(ARENA, sizeof(Var_Obj));
        var->name = expr->variable->name;
        var->value = empty_object;
    }
    return obj;
}

//возвращает объект int или float
static size_t count(wchar* str, wchar symb)
{
    size_t c = 0;
    while((str = wcschr(str, symb)) != NULL)
    {
        c++;
        str++;
    }
    return c;
}
Object* interpretate_num(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    wchar* end_ptr;
    wchar* number = expr->number->value;
    if(count(number, L'.'))
    {
        obj->kind = FLOAT_OBJ;
        obj->float_t = wcstold(number, end_ptr);
    }
    else
    {
        obj->kind = INTEGER_OBJ;
        obj->float_t = wcstoll(number, end_ptr, 10);
    }
    return obj;
}

//возвращает объект - строку
Object* interpretate_str(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    obj->kind = STRING_OBJ;
    obj->str = expr->string->value;
    return obj;
}

//возвращает объект логики
Object* interpretate_bool(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    obj->kind = BOOLEAN_OBJ;
    if(wcscmp(expr->boolean->value, L"БЛЯДЬ") == 0) obj->bool_t = 0;
    else obj->bool_t = 1;
    return obj;
}