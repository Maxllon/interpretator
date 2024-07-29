#include"interpretator.h"
#include"arena.h"

#define EXIT1 {arena_destroy(ARENA1); EXIT;}

static Arena* ARENA = NULL;
static Arena* ARENA1 = NULL;
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
        case INDEX_OBJ:
            wcscpy(DATA, L"<ИНДЕКС>");
            break;
        default:
            wprintf(L"<dev out_type()> Неизвестный тип объекта для вывода: %lld\n", obj->kind);
            EXIT1;
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
        if(wcscmp(obj->var->name, name) == 0) return obj;
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
        if(wcscmp(obj->var->name, variable->var->name) == 0)
        {
            obj->var->value = get_object(variable->var->value);
            return;
        }
    } 
    bm_vector_push(current_envi->variables, variable);
}

Object* get_object(Object* obj)
{
    if(obj == NULL) return VOID_OBJECT;
    if(obj->kind == VOID_OBJ) return obj;
    if(obj->kind == VARIABLE_OBJ) return get_object(obj->var->value);
    if(obj->kind == INDEX_OBJ)
    {
        size_t index;
        if(obj->index->index->int_t < 0)
        {
            if(obj->index->list->kind == LIST_OBJ) index = obj->index->list->list->len + obj->index->index->int_t;
            else index = wcslen(obj->index->list->str) + obj->index->index->int_t;
        }
        else index = obj->index->index->int_t;

        if(obj->index->list->kind == LIST_OBJ)
        {
            if(index >= obj->index->list->list->len)
            {
                wprintf(L"Ошибка: индекс за пределами массива\n");
                EXIT1;
            }
        }
        if(obj->index->list->kind == STRING_OBJ)
        {
            if(index >= wcslen(obj->index->list->str))
            {
                wprintf(L"Ошибка: индекс за пределами строки\n");
                EXIT1;
            }
        }

        if(obj->index->list->kind == LIST_OBJ)
        {
            return get_object(((Object*) bm_vector_at(obj->index->list->list, index)));
        }
        else
        {
            Object* ret_obj =  arena_alloc(ARENA, sizeof(Object));
            ret_obj->str =  arena_alloc(ARENA, sizeof(wchar) * (2));
            ret_obj->kind = STRING_OBJ;
            *ret_obj->str = *(obj->index->list->str + index);
            *(ret_obj->str + 1) = L'\0';
            return ret_obj;
        }
    }
    Object* ret_obj =  arena_alloc(ARENA, sizeof(Object));
    ret_obj->kind = obj->kind;
    switch(obj->kind)
    {
        case STRING_OBJ:
            ret_obj->str =  arena_alloc(ARENA, sizeof(wchar) * (1 + wcslen(obj->str)));
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
            ret_obj->list = bm_vector_create(ARENA);
            for(size_t i = 0; i < obj->list->len; ++i)
            {
                bm_vector_change(ret_obj->list, i, get_object((Object*)bm_vector_at(obj->list, i)));
            }
            break;

        default:   
            wprintf(L"<dev get_var()> Неизвестный тип объекта для получения\n");
            EXIT1;
            break;
    }
    return ret_obj;
}

/* 
точка входа в интерпретацию
*/
void interpretate(Expretion* expr, Arena* arena)
{
    ARENA1 = arena;
    ARENA = arena_create();

    current_envi = create_empty_environment(NULL);

    VOID_OBJECT =  arena_alloc(ARENA, sizeof(Object));
    VOID_OBJECT->kind = VOID_OBJ;

    DATA =  arena_alloc(ARENA, sizeof(100));

    for(size_t i = 0; i < expr->seque->expretions->len; ++i)
    {
        interpretate_atom((Expretion*) bm_vector_at(expr->seque->expretions, i));
    }
    wprintf(L"%ls\n", ((Object*)find_var(L"а", current_envi))->var->value->str);
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
        case STRING_EXPR:
            return interpretate_str(expr);
        case BOOLEAN_EXPR:  
            return interperate_bool(expr);
        case ARRAY_EXPR:
            return interpretate_list(expr);
        case INDEX_EXPR:
            return interpretate_index(expr);

        default:
            wprintf(L"<dev interpretate_atom()> Неизвестный тип объекта для интерпретации\n");
            EXIT1;
            break;
    }
}


Object* interpretate_var(Expretion* expr)
{
    Object* object = find_var(expr->variable->name, current_envi);
    if(object == NULL)
    {
        Object* obj =  arena_alloc(ARENA, sizeof(Object));
        obj->kind = VARIABLE_OBJ;
        obj->var =  arena_alloc(ARENA, sizeof(Var_Obj));
        obj->var->name =  arena_alloc(ARENA, sizeof(wchar) * (1 + wcslen(expr->variable->name)));
        wcscpy(obj->var->name, expr->variable->name);
        obj->var->value = VOID_OBJECT;
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
    Object* obj =  arena_alloc(ARENA, sizeof(Object));
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

Object* interpretate_str(Expretion* expr)
{
    Object* obj =  arena_alloc(ARENA, sizeof(Object));
    obj->kind = STRING_OBJ;
    obj->str =  arena_alloc(ARENA, sizeof(wchar) * (1 + wcslen(expr->string->value)));
    wcscpy(obj->str, expr->string->value);
    return obj;
}

Object* interperate_bool(Expretion* expr)
{
    Object* obj =  arena_alloc(ARENA, sizeof(Object));
    obj->kind = BOOLEAN_OBJ;
    obj->bool_t = wcscmp(expr->boolean->value, L"БЫЛЬ") == 0 ? 1 : 0;
    return obj;
}

Object* interpretate_list(Expretion* expr)
{
    Object* obj =  arena_alloc(ARENA, sizeof(Object));
    obj->kind = LIST_OBJ;
    obj->list = bm_vector_create(ARENA);
    for(size_t i = 0; i < expr->array->expretions->len; ++i)
    {
        bm_vector_change(obj->list, i, interpretate_atom((Expretion*) bm_vector_at(expr->array->expretions, i)));
    }
    return obj;
}

Object* interpretate_void(Expretion* expr)
{
    return VOID_OBJECT;
}

static Object* bin_plus(Object* left, Object* right)
{
    return NULL;
}

static Object* bin_minus(Object* left, Object* right)
{
    return NULL;
}

static Object* bin_mult(Object* left, Object* right)
{
    return NULL;
}

static Object* bin_del(Object* left, Object* right)
{
    return NULL;
}

static Object* bin_mod(Object* left, Object* right)
{
    return NULL;
}

static Object* bin_div(Object* left, Object* right)
{
    return NULL;
}

static Object* bin_assign(Object* left, Object* right)
{

    switch(left->kind)
    {
        case VARIABLE_OBJ:
            left->var->value = get_object(right);
            set_var(left);
            break;
        /*case INDEX_OBJ:
            switch(left->index->list->kind)
            {
                case LIST_OBJ:
                    bm_vector_change(left->index->list->list, left->index->index->int_t, get_object(right));
                default:
                    wprintf(L"Ошибка: невозможно обратиться по индексу к этому типу: %ls\n", out_type(left->index->list->var->value));
                    EXIT1;
            }
            break;*/

        default:
            wprintf(L"Ошибка: к типу %ls не возможно присвоение\n", out_type(left));
            EXIT1;
    }
    return VOID_OBJECT;
}
Object* interpretate_bin(Expretion* expr)
{
    Object* left = interpretate_atom(expr->binary->left);
    Object* right = interpretate_atom(expr->binary->right);
    wchar* op = expr->binary->op;

    if(wcscmp(op, L"=") == 0) return bin_assign(left, right);
    
    wprintf(L"Ошибка: Неизвестный оператор: <%ls>\n", op);
    EXIT1;
    return NULL;
}

Object* interpretate_index(Expretion* expr)
{
    Object* obj =  arena_alloc(ARENA, sizeof(Object));
    obj->kind = INDEX_OBJ;
    obj->index = arena_alloc(ARENA, sizeof(Index_Obj));
    obj->index->index = get_object(interpretate_atom(expr->index->index));
    obj->index->list = get_object(interpretate_atom(expr->index->destination));
    if(obj->index->index->kind != INTEGER_OBJ)
    {
        wprintf(L"Ошибка: Индекс массива представлен типа %ls\n", out_type(obj->index->index));
        EXIT1;
    }
    if(obj->index->list->kind != STRING_OBJ && obj->index->list->kind != LIST_OBJ)
    {
        wprintf(L"Ошибка: нельзя обращаться по индексу к этому типу: %ls\n", out_type(obj->index->list));
        EXIT1;  
    }
    return obj;
}

