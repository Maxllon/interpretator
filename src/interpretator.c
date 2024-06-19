#include"interpretator.h"
#include"arena.h"

static Arena* ARENA = NULL;
static Environment* current_envi = NULL;
Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = arena_alloc(ARENA, sizeof(Environment));
    envi->parent = parent;
    envi->variables = bm_vector_create(ARENA);
    return envi;
} 

Object* interpretate_var(Expretion* expr)
{
    if(find_object(current_envi, expr->variable->name) == NULL)
    {
        Object* obj = arena_alloc(ARENA, sizeof(Object));
        obj->kind = VARIABLE_OBJ;
        obj->var = arena_alloc(ARENA, sizeof(Var_Obj));
        obj->var->name = arena_alloc(ARENA, wcslen(expr->variable->name) * 2 +2);
        wcscpy(obj->var->name, expr->variable->name);
        obj->var->value = NULL;
        add_object(current_envi, obj);
    }
    return find_object(current_envi, expr->variable->name);
}

Object* interpretate_atom(Expretion* expr)
{
    switch(expr->kind)
    {
        case VARIABLE_EXPR:
            return interpretate_var(expr);
        case NUMBER_EXPR:
            return interpretate_num(expr);
        case BINARY_EXPR:
            return interpretate_bin(expr);
        default:
            wprintf(L"Ошибка интерпретации: неизвестный тип выражения.\n");
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

    Object* obj = NULL;
    for(size_t i = 0; i < expr->seque->expretions->len; ++i)
    {
        obj = interpretate_atom(bm_vector_at(expr->seque->expretions, i));
    }
    wprintf(L"%lld\n", ((Object*)find_object(current_envi, L"а"))->var->value->int_t);
    return obj;
}

Object* find_object(Environment* envi, wchar* name)
{
    for(size_t i = 0; i < envi->variables->len; ++i)
    {
        if(wcscmp(name, ((Object*)bm_vector_at(envi->variables, i))->var->name) == 0)return ((Object*)bm_vector_at(envi->variables, i));
    }
    return NULL;
}

Object* interpretate_bin(Expretion* expr)
{
    if(wcscmp(expr->binary->op, L"=") == 0)
    {
        if(expr->binary->left->kind == VARIABLE_EXPR)
        {
            interpretate_var(expr->binary->left)->var->value = interpretate_num(expr->binary->right);
            return find_object(current_envi, expr->binary->left->variable->name);
        }
        wprintf(L"Ошибка: недопустимое выражение слева от оператора: %ls\n", expr->binary->op);
        EXIT;

    }
    wprintf(L"Ошибка: неизвестный оператор: %ls\n", expr->binary->op);
    EXIT;
}

Object* interpretate_num(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    obj->kind = INTEGER_OBJ;
    wchar* end_ptr;
    obj->int_t = wcstoll(expr->number->value, &end_ptr, 10);
    return obj;
}

void add_object(Environment* envi, Object* obj)
{
    bm_vector_push(envi->variables, obj, ARENA);
}
