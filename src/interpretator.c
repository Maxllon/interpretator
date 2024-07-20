#include"interpretator.h"
#include"arena.h"

static Arena* ARENA = NULL;
static Object* VOID_OBJECT = NULL;
static Environment* current_envi = NULL;

static wchar* DATA;
wchar* out_type(Object* obj)
{
    switch(obj->kind)
    {
        case VOID_OBJ:
            wcscpy(DATA, L"<ПУСТОТА>");
            break;
        case INTEGER_OBJ:
            wcscpy(DATA, L"<ЦЕЛОЕ ЧИСЛО>");
            break;
        case FLOAT_OBJ:
            wcscpy(DATA, L"<ДРОБНОЕ ЧИСЛО>");
            break;
        case STRING_OBJ:
            wcscpy(DATA, L"<СТРОКА>");
            break;
        case LIST_OBJ:
            wcscpy(DATA, L"<СПИСОК>");
            break;
        default:
            wcscpy(DATA, L"<dev out_type()> Неизвестный тип объекта для вывода\n");
            EXIT;
            break;
    }
    return DATA;
}

Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = arena_alloc(ARENA, sizeof(Environment));
    envi->variables = bm_vector_create(ARENA);
    envi->parent = parent;
    return envi;
}

Object* find_var(wchar* name, Environment* envi)
{
    Object* obj;
    for(size_t i = 0; i < envi->variables->len; ++i)
    {
        obj = (Object*)bm_vector_at(envi->variables, i);
        if(wcscmp(obj->var.name, name) == 0) return obj->var.value;
    } 
    if(envi->parent != NULL) return find_var(name, envi->parent);
    return NULL;
}

void set_var(Object* variable)
{
    Object* obj;
    for(size_t i = 0; i < current_envi->variables->len; ++i)
    {
        obj = (Object*)bm_vector_at(current_envi->variables, i);
        if(wcscmp(obj->var.name, variable->var.name) == 0)
        {
            obj->var.value = variable->var.value;
            return;
        }
    } 
    bm_vector_push(current_envi->variables, variable);
}

Object* get_object(Object* obj)
{
    if(obj->kind == VOID_OBJ) return obj;
    if(obj->kind == VARIABLE_OBJ) return get_object(obj->var.value);
    Object* ret_obj = arena_alloc(ARENA, sizeof(Object));
    ret_obj->kind = obj->kind;
    switch(obj->kind)
    {
        case STRING_OBJ:
            ret_obj->str = arena_alloc(ARENA, sizeof(wchar) * (1 + wcslen(obj->str)));
            wcscpy(ret_obj->str, obj->str);
            break;

        case BOOLEAN_OBJ:
            ret_obj->bool_t = obj->bool_t;
            break;

        case INTEGER_OBJ:
            ret_obj->int_t = obj->int_t;
            break;

        case FLOAT_OBJ:
            ret_obj->float_t = obj->float_t;
            break;

        case LIST_OBJ:
            ret_obj->kind = LIST_OBJ;
            ret_obj->list = bm_vector_create(ARENA);
            for(size_t i = 0; i < obj->list->len; ++i)
            {
                bm_vector_change(ret_obj->list, i, get_object((Object*)bm_vector_at(obj->list, i)));
            }
            break;

        default:   
            wprintf(L"<dev get_var()> Неизвестный тип объекта для получения\n");
            EXIT;
            break;
    }
    return ret_obj;
}

/* 
точка входа в интерпретацию
*/
void interpretate(Expretion* expr, Arena* arena)
{
    ARENA = arena;

    current_envi = create_empty_environment(NULL);

    VOID_OBJECT = arena_alloc(ARENA, sizeof(Object));
    VOID_OBJECT->kind = VOID_OBJ;

    DATA = arena_alloc(ARENA, sizeof(100));

    for(size_t i = 0; i < expr->seque->expretions->len; ++i)
    {
        interpretate_atom((Expretion*) bm_vector_at(expr->seque->expretions, i));
    }


    wprintf(L"%LF\n", ((Object*)find_var(L"а", current_envi))->float_t);

}

Object* interpretate_atom(Expretion* expr)
{
    switch(expr->kind)
    {
        case NUMBER_EXPR:
            return interpetate_num(expr);
        case VARIABLE_EXPR:
            return interpretate_var(expr);
        case BINARY_EXPR:
            return interpretate_bin(expr);
        default:
            wprintf(L"<dev interpretate_atom()> Неизвестный тип объекта для интерпретации\n");
            EXIT;
            break;
    }
}


Object* interpretate_var(Expretion* expr)
{
    Object* object = find_var(expr->variable->name, current_envi);
    if(object == NULL)
    {
        Object* obj = arena_alloc(ARENA, sizeof(Object));
        obj->kind = VARIABLE_OBJ;
        obj->var.name = expr->variable->name;
        obj->var.value = VOID_OBJECT;
        set_var(obj);
        return obj;
    }
    return object;
}


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
Object* interpetate_num(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    wchar* end_ptr;
    if(count(expr->number->value, L'.') == 1)
    {
        obj->kind = FLOAT_OBJ;
        obj->float_t = wcstold(expr->number->value, &end_ptr);
        return obj;
    }
    obj->kind = INTEGER_OBJ;
    obj->int_t = wcstoll(expr->number->value, &end_ptr, 10);
    return obj;
}

Object* interpretate_bin(Expretion* expr)
{
    Object* left = interpretate_atom(expr->binary->left);
    Object* right = interpretate_atom(expr->binary->right);
    wchar* op = expr->binary->op;

    if(wcscmp(op, L"=") == 0)
    {
        switch(left->kind)
        {
            case VARIABLE_OBJ:
                left->var.value = get_object(right);
                set_var(left);
                break;

            default:
                wprintf(L"Ошибка: к типу <%ls> не возможно присвоение\n", out_type(left));
                EXIT;
        }
        return VOID_OBJECT;
    }
}

