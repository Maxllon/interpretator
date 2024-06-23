#include"interpretator.h"
#include"arena.h"

static Arena* ARENA = NULL;
static Environment* current_envi = NULL;
static Object* Null_object = NULL;
Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = arena_alloc(ARENA, sizeof(Environment));
    envi->parent = parent;
    envi->variables = bm_vector_create(ARENA);
    return envi;
} 

Object* interpretate_var(Expretion* expr)
{
    wchar* name = expr->variable->name;
    Object* obj = find_object(current_envi, name);
    if(obj == NULL)
    {
        obj = arena_alloc(ARENA, sizeof(Object));
        obj->var = arena_alloc(ARENA, sizeof(Var_Obj));
        obj->var->name = arena_alloc(ARENA, 2 * (wcslen(name) + 1));
        wcscpy(obj->var->name, name);
        obj->var->value = Null_object;
        return obj;
    }
    return obj;
}

Object* interpretate_atom(Expretion* expr)
{
    switch(expr->kind)
    {
        case VARIABLE_EXPR:
            return interpretate_var(expr);
        default:
            wprintf("Ошибка интерпретации: неизвестный тип выражения: %d\n", expr->kind);
            EXIT;
    }
    return NULL;
}


Object* interpretate(Expretion* expr, Arena* arena)
{

    ARENA = arena;
    current_envi = arena_alloc(ARENA, sizeof(Environment));
    current_envi->parent = NULL;
    current_envi->variables = bm_vector_create(ARENA);
    Null_object = arena_alloc(ARENA, sizeof(Object*));
    Null_object->kind = EMPTY_OBJ;

    Object* obj = NULL;
    for(size_t i = 0; i < expr->seque->expretions->len; ++i)
    {
        obj = interpretate_atom(bm_vector_at(expr->seque->expretions, i));
    }
    return obj;
}

Object* find_object(Environment* envi, wchar* name)
{
    for(size_t i = 0; i < envi->variables->len; ++i)
    {
        if(wcscmp(name, ((Object*)bm_vector_at(envi->variables, i))->var->name) == 0) return ((Object*)bm_vector_at(envi->variables, i));
    }
    if(envi->parent != NULL) return find_object(envi->parent, name);
    return NULL;
}

Object* get_object(Environment* envi, wchar* name)
{
    for(size_t i = 0; i < envi->variables->len; ++i)
    {
        if(wcscmp(name, ((Object*)bm_vector_at(envi->variables, i))->var->name) == 0)
        {
            Object* obj = ((Object*)bm_vector_at(envi->variables, i));
            switch(obj->kind)
            {
                default:
                    wprintf("Не могу найти обьект типа: %d\n", obj->kind);
                    EXIT;
            }
        }
    }
    if(envi->parent != NULL) return get_object(envi->parent, name);
    return NULL;
}

void add_object(Environment* envi, Object* obj)
{
    bm_vector_push(envi->variables, obj, ARENA);
}
