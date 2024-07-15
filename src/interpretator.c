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

//возвращает есть ли переменная в системе
int is_def(Environment* envi, wchar* name)
{
    for (int i = 0; i < envi->variables->len; ++i) 
    {
        Var_Obj* var_obj = (Var_Obj*)bm_vector_at(envi->variables, i);
        if (wcscmp(var_obj->name, name) == 0) 
        {
            return 1;
        }
    }
    if (envi->parent != NULL) 
    {
        return is_def(envi->parent, name);
    }
    return 0;
}

/*
ищет переменную в системе
возвращает ссылку на ее значение
*/
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
    return empty_object;
}

//устанавливает переменной конкретный объект и возвращет его
Object* set_variable(Environment* envi, wchar* name, Object* obj)
{
    for (int i = 0; i < envi->variables->len; ++i) 
    {
        Var_Obj* var_obj = (Var_Obj*)bm_vector_at(envi->variables, i);
        if (wcscmp(var_obj->name, name) == 0) 
        {
            var_obj->value = obj;
            bm_vector_change(envi->variables, i, var_obj);
            return obj;
        }
    }
    if (envi->parent != NULL) 
    {
        return set_variable(envi->parent, name, obj);
    }
    return empty_object;
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
        case BOOLEAN_OBJ:
            new_obj->bool_t = obj->bool_t;
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
        case BOOLEAN_OBJ:
            return copy_object(obj);
        case ARRAY_OBJ:
            return obj;
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
    wprintf(L"%ls\n", find_variable(envi, L"ж")->str);
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
        case BOOLEAN_EXPR:
            return interpretate_bool(expr);
        case BINARY_EXPR:
            return interpretate_bin(expr);
        case ARRAY_EXPR:    
            return interpretate_list(expr);
        case INDEX_EXPR:
            return interpretate_index(expr);
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
    wchar* name = expr->variable->name;
    Object* obj = find_variable(current_envi, expr->variable->name);
    if(!is_def(current_envi, name))
    {
        Var_Obj* var = arena_alloc(ARENA, sizeof(Var_Obj));
        var->name = expr->variable->name;
        var->value = empty_object;
        add_variable(current_envi, var);
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
        obj->float_t = wcstold(number, &end_ptr);
    }
    else
    {
        obj->kind = INTEGER_OBJ;
        obj->int_t = wcstoll(number, &end_ptr, 10);
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


static Object* number_operators(Object* left, Object* right, wchar* op)
{
    Object* result = arena_alloc(ARENA, sizeof(Object));
    
    if(left->kind == FLOAT_OBJ || right->kind == FLOAT_OBJ) result->kind = FLOAT_OBJ;
    else result->kind = INTEGER_OBJ;
    
    if(result->kind == INTEGER_OBJ)
    {
        if(wcscmp(op, L"+") == 0)
        {
            result->int_t = left->int_t + right->int_t;
            return result;
        }
        if(wcscmp(op, L"-") == 0)
        {
            result->int_t = left->int_t + right->int_t;
            return result;
        }
        if(wcscmp(op, L"*") == 0)
        {
            result->int_t = left->int_t * right->int_t;
            return result;
        }
        if(wcscmp(op, L"/") == 0)
        {
            result->kind = FLOAT_OBJ;
            result->float_t= (long double)left->int_t / (long double)right->int_t;
            return result;
        }
        if(wcscmp(op, L"^") == 0)
        {
            result->int_t = powl((long double) left->int_t, (long double) right->int_t);
            return result;
        }
        if(wcscmp(op, L"\%") == 0)
        {
            result->int_t = left->int_t % right->int_t;
            return result;
        }
        if(wcscmp(op, L"//") == 0)
        {
            result->int_t =  left->int_t / right->int_t;
            return result;
        }

        //логические
        if(wcscmp(op, L"==") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = left->int_t == right->int_t;
            return result;
        }
        if(wcscmp(op, L"!=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = left->int_t != right->int_t;
            return result;
        }
        if(wcscmp(op, L">=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = left->int_t >= right->int_t;
            return result;
        }
        if(wcscmp(op, L"<=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = left->int_t <= right->int_t;
            return result;
        }
        if(wcscmp(op, L">") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = left->int_t > right->int_t;
            return result;
        }
        if(wcscmp(op, L"<") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = left->int_t < right->int_t;
            return result;
        }

    }
    if(result->kind == FLOAT_OBJ)
    {
        if(wcscmp(op, L"+") == 0)
        {
            result->float_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t)
            +
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"-") == 0)
        {
            result->float_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t)
            -
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"*") == 0)
        {
            result->float_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t)
            *
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"/") == 0)
        {
            result->float_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t)
            /
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"^") == 0)
        {
            result->float_t = powl(
                (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t),
                (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t));
            return result;
        }
        if(wcscmp(op, L"\%") == 0)
        {
            result->float_t = fmodl(
                (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t),
                (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t));
            return result;
        }
        if(wcscmp(op, L"//") == 0)
        {
            result->kind = INTEGER_OBJ;
            result->int_t = (long long)
            ((left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t)
            /
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t));
            return result;
        }

        //логические
        if(wcscmp(op, L"==") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t) 
            ==
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"!=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t) 
            !=
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L">=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t) 
            >=
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"<=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t) 
            <=
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L">") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t) 
            >
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
        if(wcscmp(op, L"<") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = 
            (left->kind == INTEGER_OBJ ? (long double)left->int_t : left->float_t) 
            <
            (right->kind == INTEGER_OBJ ? (long double)right->int_t : right->float_t);
            return result;
        }
    }
    wprintf(L"Неподходящий оператор для работы с числами!\n");
    EXIT;
    return NULL;
}

static Object* boolean_operators(Object* left, Object* right, wchar* op)
{
    Object* result = arena_alloc(ARENA, sizeof(Object));
    result->kind = BOOLEAN_OBJ;
    if(wcscmp(op, L"==") == 0)
    {
        result->bool_t = left->bool_t == right->bool_t;
        return result;
    }
    if(wcscmp(op, L"!=") == 0)
    {
        result->bool_t = left->bool_t != right->bool_t;
        return result;
    }
    if(wcscmp(op, L"и") == 0)
    {
        result->bool_t = left->bool_t && right->bool_t;
        return result;
    }
    if(wcscmp(op, L"або") == 0)
    {
        result->bool_t = left->bool_t || right->bool_t ? 1 : 0;
        return result;
    }

    wprintf(L"Неподходящий оператор для работы с числами!\n");
    EXIT;
    return NULL;
}

static Object* string_operators(Object* left, Object* right, wchar* op)
{
    Object* result = arena_alloc(ARENA, sizeof(Object));
    if(left->kind == STRING_OBJ && right->kind == STRING_OBJ)
    {
        if(wcscmp(op, L"==") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = wcscmp(left->str, right->str) == 0 ? 1 : 0;
            return result;
        }
        if(wcscmp(op, L"!=") == 0)
        {
            result->kind = BOOLEAN_OBJ;
            result->bool_t = wcscmp(left->str, right->str) == 0 ? 0 : 1;
            return result;
        }
        if(wcscmp(op, L"+") == 0)
        {
            result->kind = STRING_OBJ;
            result->str = arena_alloc(ARENA, sizeof(wchar) * (wcslen(left->str) + wcslen(right->str) + 1));
            wcscpy(result->str, left->str);
            wcscat(result->str, right->str);
            return result;
            
        }
    }
    if(left->kind == INTEGER_OBJ || right->kind == INTEGER_OBJ)
    {
        result->kind = STRING_OBJ;
        if(left->kind == INTEGER_OBJ)
        {
            Object* temp = left;
            left = right;
            right = temp;
        }
        if(right->int_t < 0)
        {
            wprintf(L"Нельзя умножить строку на число, которое меньше нуля!!\n");
            EXIT;
        }
        if(wcscmp(op, L"*") == 0)
        {
            size_t len = wcslen(left->str);
            result->str = arena_alloc(ARENA, sizeof(wchar) * ((wcslen(left->str) * right->int_t) + 1));
            size_t i,j;
            size_t index = 0;
            for(i = 0; i < right->int_t; ++i)
            {
                for(j=0;j<len;++j)
                {
                    *(result->str + index++) = *(left->str + j);
                }
            }
            *(result->str + index) = L'\0';
            return result;
        }
    }
    wprintf(L"Неподходящий оператор для работы с строками!\n");
    EXIT;
    return NULL;
}

Object* interpretate_bin(Expretion* expr)
{
    Object* left = interpretate_atom(expr->binary->left);
    Object* right = interpretate_atom(expr->binary->right);
    wchar* op = expr->binary->op;
    if(wcscmp(op, L"=") == 0)
    {
        if(expr->binary->left->kind != VARIABLE_EXPR)
        {
            wprintf(L"Слева от оператора присвоения должно находиться имя переменной!\n");
            EXIT;
        }
        return set_variable(current_envi, expr->binary->left->variable->name, get_object(right));
    }
    if((left->kind == FLOAT_OBJ || left->kind == INTEGER_OBJ) && (right->kind == FLOAT_OBJ || right->kind == INTEGER_OBJ)) return number_operators(left, right, op);
    if(left->kind == BOOLEAN_OBJ && right->kind == BOOLEAN_OBJ) return boolean_operators(left, right, op);
    if(left->kind == STRING_OBJ || right->kind == STRING_OBJ) return string_operators(left, right, op);

    if(wcscmp(op, L"==") == 0 && left->kind != right->kind)
    {
        Object* obj = arena_alloc(ARENA, sizeof(Object));
        obj->kind = BOOLEAN_OBJ;
        obj->bool_t = 0;
        return obj;
    }


    wprintf(L"Неизвестный оператор: %ls\n", op);
    EXIT;
}

Object* interpretate_index(Expretion* expr)
{
    Object* ret;
    Object* dest = get_object(interpretate_atom(expr->index->destination));
    Object* index = get_object(interpretate_atom(expr->index->index));
    if(index->kind != INTEGER_OBJ)
    {
        wprintf(L"Ошибка: обращение к элементам списка возможно только по целочисленному ключу.\n");
        EXIT;
    }
    if(dest->kind != ARRAY_OBJ && dest->kind != STRING_OBJ)
    {
        wprintf(L"Ошибка: Нельзя обращаться по индексу к этому типу данных.\n");
        EXIT;
    }
    if(dest->kind == STRING_OBJ)
    {
        if(index->int_t < 0) index->int_t = wcslen(dest->str) + index->int_t;
        if(index->int_t > wcslen(dest->str) - 1 || index->int_t < 0)
        {
            wprintf(L"Ошибка: индекс за пределами списка!\n");
            EXIT;
        }
        ret = arena_alloc(ARENA, sizeof(Object));
        ret->kind = STRING_OBJ;
        ret->str = arena_alloc(ARENA, sizeof(wchar) * (2));
        *(ret->str) = *(dest->str + index->int_t);
        return ret;
    }
    if(dest->kind == ARRAY_OBJ)
    {
        if(index->int_t < 0) index->int_t = dest->array->len + index->int_t;
        if(index->int_t > dest->array->len - 1 || index->int_t < 0)
        {
            wprintf(L"Ошибка: индекс за пределами списка!\n");
            EXIT;
        }
        return get_object((Object*)bm_vector_at(dest->array, index->int_t));
    }
    return NULL;
}
Object* interpretate_list(Expretion* expr)
{
    Object* object = arena_alloc(ARENA, sizeof(Object));
    object->kind = ARRAY_OBJ;
    object->array = bm_vector_create(ARENA);
    for(size_t i = 0; i < expr->array->expretions->len; ++i)
    {
        bm_vector_push(object->array, get_object(interpretate_atom(bm_vector_at(expr->array->expretions, i))));
    }
    return object;
}