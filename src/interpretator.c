#include"interpretator.h"
#include"arena.h"

static Arena* ARENA = NULL;
static Environment* current_envi = NULL;
static Object* Null_object = NULL;
static Object* True_object = NULL;
static Object* False_object = NULL;

/*
получает ссылку на родителя-пространство_имен и создает пустое пространство имен
*/
Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = arena_alloc(ARENA, sizeof(Environment));
    envi->parent = parent;
    envi->variables = bm_vector_create(ARENA);
    return envi;
} 

/*
если переменная есть в текущем пространстве имен, возвращает ссылку на нее
иначе создает переменную, которая указывает на пустой обьект, и возвращает ссылку на нее
*/

Object* interpretate_var(Expretion* expr)
{
    wchar* name = expr->variable->name;
    Object* obj = find_object(current_envi, name);
    if(obj == NULL)
    {
        obj = arena_alloc(ARENA, sizeof(Object));
        obj->kind = VARIABLE_OBJ;

        obj->var = arena_alloc(ARENA, sizeof(Var_Obj));
        obj->var->name = arena_alloc(ARENA, 2 * (wcslen(name) + 1));
        wcscpy(obj->var->name, name);

        obj->var->value = Null_object;
    }
    return obj;
}

/*
интерпретирует выражение в обьект и возвращает ссылку на сам объект
*/
Object* interpretate_atom(Expretion* expr)
{
    switch(expr->kind)
    {
        case VARIABLE_EXPR:
            return interpretate_var(expr);
        case NUMBER_EXPR:
            return interpretate_num(expr);
        case VOID_EXPR:
            return interpretate_void();
        case ARRAY_EXPR:
            return interpretate_list(expr);
        case STRING_EXPR:
            return interpretate_str(expr);
        case BINARY_EXPR:
            return interpretate_bin(expr);
        default:
            wprintf(L"Ошибка интерпретации: неизвестный тип выражения: %d\n", expr->kind);
            EXIT;
    }
    return NULL;
}

/*
проходит по списку всех выражений и интерпретирует их по одному
возвращает результат интерпретации последнего выражения
*/
Object* interpretate(Expretion* expr, Arena* arena)
{
    //инициализация глобальных переменных
    ARENA = arena;
    current_envi = create_empty_environment(NULL);
    Null_object = arena_alloc(ARENA, sizeof(Object*));
    Null_object->kind = EMPTY_OBJ;

    True_object = arena_alloc(ARENA, sizeof(Object*));
    True_object->kind = TRUE_OBJECT;

    False_object = arena_alloc(ARENA, sizeof(Object*));
    False_object->kind = FALSE_OBJECT;

    Object* obj = NULL;
    for(size_t i = 0; i < expr->seque->expretions->len; ++i)
    {
        obj = interpretate_atom(bm_vector_at(expr->seque->expretions, i));
    }
    return obj;
}

static int is_number(Object* object)
{
    if(object->kind == INTEGER_OBJ || object->kind == FLOAT_OBJ) return 1;
    return 0;
}

static Object* num_op(Object* left, Object* right, wchar* op)
{
    Object* ret_obj = arena_alloc(ARENA, sizeof(Object));
    size_t prior = find_priority(op);
    if(prior == 4)
    {
        if(wcscmp(op, L"=="))
        {
            if(left->kind == INTEGER_OBJ) ret_obj->bool_t = (left->int_t == right->int_t);
            else ret_obj->bool_t = (left->float_t == right->float_t);
        }
        else if(wcscmp(op, ">"))
        {
            long double left_n, right_n;
            if(left->kind == INTEGER_OBJ) left_n = (long double)left->int_t;
            else left_n = left->float_t;
        }
    }
    
}

//возвращает результат бинарной операции
Object* interpretate_bin(Expretion* expr)
{
    wchar* op = expr->binary->op;

    Object* ret_obj = arena_alloc(ARENA, sizeof(Object));
    Object* left = interpretate_atom(expr->binary->left);
    Object* right = interpretate_atom(expr->binary->right);

    if(wcscmp(op, L"=") == 0)
    {
        if(left->kind != VARIABLE_OBJ)
        {
            wprintf(L"Слева от оператора присвоения должна быть переменная!");
            EXIT;
        }
        left->var->value = get_object(right);
        return left;
    }
    left = get_object(left);
    right = get_object(right);
    if(is_number(left) && is_number(right)) return num_op(left, right, op);

    wprintf(L"выражение невозможно с оператором: %ls\n", op);
    EXIT;
}

/*
возвращает число типа, которого оно является
*/
Object* interpretate_num(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    wchar c;
    wchar* end_ptr;
    for(size_t i = 0; (c = *(expr->number->value + i)) != L'\0'; ++i)
    {
        if(c == L'.')
        {
            obj->kind = FLOAT_OBJ;
            obj->float_t = wcstold(expr->number->value, &end_ptr);
            return obj;
        }
    }
    obj->kind = INTEGER_OBJ;
    obj->float_t = wcstoll(expr->number->value, &end_ptr, 10);
    return obj;
}

/*
ищет переменную в заданном пространстве имен
возвращает ссылку на нее или NULL если переменной с заданным именем нет
*/
Object* find_object(Environment* envi, wchar* name)
{
    for(size_t i = 0; i < envi->variables->len; ++i)
    {
        if(wcscmp(name, ((Object*)bm_vector_at(envi->variables, i))->var->name) == 0) return ((Object*)bm_vector_at(envi->variables, i));
    }
    if(envi->parent != NULL) return find_object(envi->parent, name);
    return NULL;
}

/*
получает на вход выражение
возвращает обьект-строку
*/
Object* interpretate_str(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    obj->kind = STRING_OBJ;
    obj->str = arena_alloc(ARENA, 2 * (wcslen(expr->string->value) + 1));
    wcscpy(obj->str, expr->string->value);
    return obj;
}

/*
получает на вход выражение
возвращает обьект-логика
*/
Object* interpretate_bool(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    obj->kind = BOOLEAN_OBJ;
    if(wcscmp(expr->boolean->value, L"БЛЯДЬ") == 0) obj->bool_t = 0;
    else obj->bool_t = 1;
    return obj;
}

/*
получает на вход выражение
возвращает обьект-список
*/
Object* interpretate_list(Expretion* expr)
{
    Object* obj = arena_alloc(ARENA, sizeof(Object));
    obj->kind = LIST_OBJ;
    obj->list = bm_vector_create(ARENA);
    for(size_t i = 0; i < expr->array->expretions->len; ++i)
    {
        bm_vector_push(obj->list, get_object(interpretate_atom((Expretion*)bm_vector_at(expr->array->expretions, i))));
    }
    return obj;
}

/*
возвращает ссылку на пустой объект
*/
Object* interpretate_void()
{
    return Null_object;
}

/*
получает на вход переменную
решает вернуть ссылку на объект или ссылку на его копию
если получает не объект переменной, то возвращает его
*/
Object* get_object(Object* obj)
{
    if(obj->kind != VARIABLE_OBJ) return obj;
    Object* object = obj->var->value;
    if(object->kind == EMPTY_OBJ)
    {
        wprintf(L"Нельзя обращаться к переменной <%ls>, пока она пустая!!!\n", obj->var->name);
        EXIT;
    }
    switch(object->kind)
    {
        case EMPTY_OBJ:
            object = Null_object;
            break;
        default:
            wprintf(L"Не могу получить доступа к объекту!!\n");
            EXIT;
    }
    return object;
}

/*
добавляет в текущее пространство имен переменную
*/
void add_object(Object* obj)
{
    if(obj->kind != VARIABLE_OBJ)
    {
        wprintf(L"Ожидался объект типа переменная, но была попытка добавить в пространство имен: %d\n", obj->kind);
        EXIT;
    }
    bm_vector_push(current_envi->variables, obj);
}
