#include"interpretator.h"

static Arena* ARENA;
static module* CURR_MODULE;

void interpretate(Expression* expr, Arena* arena)
{
    if(expr == NULL || arena == NULL) return;
    ARENA = arena;
    CURR_MODULE = new_module(arena, NULL);
    interpretate_atom(expr);
    variable* var = find_var(CURR_MODULE, bmpl_string_from_str(L"банан", ARENA), ARENA);
    wprintf(L"%ls\n", str_from_big_num(var->value->num, ARENA));
}

bmpl_object* interpretate_atom(Expression* expr)
{
    bmpl_object* res;
    switch(expr->kind)
    {
        case VARIABLE_EXPR:
            res = interpretate_var(expr);
            break;
        case BINARY_EXPR:
            res = interpretate_binary(expr);
            break;
        case NUMBER_EXPR:
            res = interpretate_num(expr);
            break;
        case STRING_EXPR:
            res = interpretate_str(expr);
            break;
        case SEQUE_EXPR:
            res = interpretate_seque(expr);
            break;
        case BOOLEAN_EXPR:
            res = interpretate_bool(expr);
            break;
        case UNARY_EXPR:
            res = interpretate_unary(expr);
            break;
        case ARRAY_EXPR:
            res = interpretate_array(expr);
            break;
        default:
            wprintf(L"Интерпретация: неизвестный тип выражения: %d\n", expr->kind);
            EXIT;
            break;
    }
    return res;
}
bmpl_object* interpretate_seque(Expression* expr)
{
    for(size_t i = 0; (Expression*)bm_vector_at(expr->seque->expressions, i) != NULL; ++i)
    {
        interpretate_atom((Expression*)bm_vector_at(expr->seque->expressions, i));
    }
    return new_object(VOID_OBJ, NULL, ARENA);
}
/*bmpl_object* interpretate_index(Expression* expr)
{
    bmpl_object* num = interpretate_atom(expr->index->index);
    if(num->type != NUM_OBJ)
    {
        wprintf(L"Интерпретатор: индекс может быть только числом\n");
        EXIT;
    }
    if(*(size_t*)bm_vector_at(num->num->digits, 0) != 0)
    {
        wprintf(L"Интерпретатор: индекс не может быть дробным\n");
        EXIT;
    }
    bmpl_object* dest = interpretate_atom(expr->index->destination);
    switch(dest->type)
    {
        case STR_OBJ:
            big_num* pos = num->num;
            if(num->num->is_negative) pos = sum_big(dest->str->size, num, ARENA);
            if(pos->is_negative || )
            {
                wprintf(L"Интерпретатор: индекс не может принимать отрицательное значение\n");
                EXIT;
            }
            size_t ps = ull_from_big_num(pos);
            wchar* str = arena_alloc(ARENA, sizeof(wchar)*2);
            *str = dest

        default:
            wprintf(L"Обращаться по индексу можно только к спискам и строкам\n");
            EXIT;
    }
}*/
bmpl_object* interpretate_array(Expression* expr)
{
    dk_node* root = NULL;
    for(size_t i=0;(Expression*)bm_vector_at(expr->array->expressions, i) != NULL; ++i)
    {
        root = dk_merge(root, dk_new_node(copy_object(interpretate_atom((Expression*)bm_vector_at(expr->array->expressions, i)), ARENA), ARENA), ARENA);
    }
    return new_object(LIST_OBJ, root, ARENA);
}
bmpl_object* un_bool(wchar* op, int value)
{
    int res;
    if(wcscmp(L"НЕ", op) == 0)
    {
        res = !value;
        return new_object(BOOL_OBJ, &res, ARENA);
    }

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\"", op, get_object_type(BOOL_OBJ)->str->string);
    EXIT;
}
bmpl_object* un_num(wchar* op, big_num* value)
{
    big_num* res;
    if(wcscmp(L"-", op) == 0)
    {
        res = big_num_from_str(str_from_big_num(value, ARENA), ARENA);
        res->is_negative = !res->is_negative;
        return new_object(NUM_OBJ, res, ARENA);
    }

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\"", op, get_object_type(NUM_OBJ)->str->string);
    EXIT;
}
bmpl_object* interpretate_unary(Expression* expr)
{
    wchar* op = expr->unary->op;
    bmpl_object* value = interpretate_atom(expr->unary->value);
    if(value->type == BOOL_OBJ) return un_bool(op, value->_bool);
    if(value->type == NUM_OBJ) return un_num(op, value->num);

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\"", op, get_object_type(value->type)->str->string);
    EXIT;
}
bmpl_object* interpretate_bool(Expression* expr)
{
    int res = wcscmp(L"Истина", expr->boolean->value) == 0;
    return new_object(BOOL_OBJ, &res, ARENA);
}
bmpl_object* interpretate_var(Expression* expr)
{
    variable* res = find_var(CURR_MODULE, bmpl_string_from_str(expr->variable->name, ARENA), ARENA);
    if(!res)
    {
        wprintf(L"Интерпретация: не найдено переменной под именем: \"%ls\"\n", expr->variable->name);
        EXIT;
    }
    return res->value;
}
bmpl_object* interpretate_num(Expression* expr)
{
    return new_object(NUM_OBJ, big_num_from_str(expr->number->value, ARENA), ARENA);
}
bmpl_object* interpretate_str(Expression* expr)
{
    return new_object(STR_OBJ, bmpl_string_from_str(expr->string->value, ARENA), ARENA);
}

bmpl_object* bin_num_num(wchar* op, big_num* left, big_num* right)
{
    if(wcscmp(op, L"+") == 0) return new_object(NUM_OBJ, sum_big(left, right, ARENA), ARENA);
    if(wcscmp(op, L"-") == 0) return new_object(NUM_OBJ, sub_big(left, right, ARENA), ARENA);
    if(wcscmp(op, L"*") == 0) return new_object(NUM_OBJ, mult_big(left, right, ARENA), ARENA);
    if(wcscmp(op, L"/") == 0) return new_object(NUM_OBJ, div_big(left, right, ARENA), ARENA);
    if(wcscmp(op, L"//") == 0) return new_object(NUM_OBJ, div_int_big(left, right, ARENA), ARENA);
    if(wcscmp(op, L"\%") == 0) return new_object(NUM_OBJ, mod_big(left, right, ARENA), ARENA);

    int res = compare_big_normal(left, right);
    if(wcscmp(op, L"==") == 0)
    {
        res = (res == 0) ? 1 : 0;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(op, L">") == 0)
    {
        res = (res == 1) ? 1 : 0;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(op, L"<") == 0)
    {
        res = (res == -1) ? 1 : 0;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(op, L">=") == 0)
    {
        res = (res >= 0) ? 1 : 0;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(op, L"<=") == 0)
    {
        res = (res <= 0) ? 1 : 0;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(op, L"!=") == 0)
    {
        res = (res != 0) ? 1 : 0;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(NUM_OBJ)->str->string, op, get_object_type(NUM_OBJ)->str->string);
    EXIT;
}
bmpl_object* bin_bool_bool(wchar* op, int left, int right)
{
    int res;
    if(wcscmp(L"==", op) == 0)
    {
        res = left == right;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(L"ИЛИ", op) == 0)
    {
        res = left || right;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    if(wcscmp(L"И", op) == 0)
    {
        res = left && right;
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(BOOL_OBJ)->str->string, op, get_object_type(BOOL_OBJ)->str->string);
    EXIT;
}
bmpl_object* bin_str_str(wchar* op, bmpl_string* left, bmpl_string* right)
{
    if(wcscmp(L"+", op) == 0) return new_object(STR_OBJ, bmpl_string_cancat(left, right, ARENA), ARENA);
    if(wcscmp(L"==", op) == 0)
    {
        int res = bmpl_string_equal(left, right);
        return new_object(BOOL_OBJ, &res, ARENA);
    }
    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(STR_OBJ)->str->string, op, get_object_type(STR_OBJ)->str->string);
    EXIT;
}

bmpl_object* bin_str_num(wchar* op, bmpl_string* left, big_num* right)
{
    if(wcscmp(L"*", op) == 0) return new_object(STR_OBJ, bmpl_string_mult(left, right, ARENA), ARENA);

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(STR_OBJ)->str->string, op, get_object_type(NUM_OBJ)->str->string);
    EXIT;
}

bmpl_object* assign(Expression* expr)
{
    if(expr->binary->left->kind == VARIABLE_EXPR)
    {
        bmpl_string* name = bmpl_string_from_str(expr->binary->left->variable->name, ARENA);
        bmpl_object* obj = copy_object(interpretate_atom(expr->binary->right), ARENA);
        variable* var = find_var(CURR_MODULE, name, ARENA);
        if(!var)
        {
            var = new_variable(name, obj, ARENA);
            add_var(CURR_MODULE, var, ARENA);
        }
        else var->value = obj;
        return obj;
    }
    wprintf(L"Интерпретация: неподходящий тип левого выражения в присваивании\n");
    EXIT;
    return NULL;
    
}

bmpl_object* interpretate_binary(Expression* expr)
{
    wchar* op = expr->binary->op;

    if(wcscmp(op, L"=") == 0) return assign(expr);

    bmpl_object* left = interpretate_atom(expr->binary->left);
    bmpl_object* right = interpretate_atom(expr->binary->right);

    if(left->type == NUM_OBJ && right->type == NUM_OBJ) return bin_num_num(op, left->num, right->num);
    if(left->type == STR_OBJ && right->type == STR_OBJ) return bin_str_str(op, left->str, right->str);
    if((left->type == NUM_OBJ || right->type == NUM_OBJ) && (left->type == STR_OBJ || right->type == STR_OBJ))
    {
        if(left->type == NUM_OBJ)
        {
            bmpl_object* temp = left;
            left = right;
            right = temp;
        }
        return bin_str_num(op, left->str, right->num);
    }
    if(left->type == BOOL_OBJ && right->type == BOOL_OBJ) return bin_bool_bool(op, left->_bool, right->_bool);
    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(left->type)->str->string, op, get_object_type(right->type)->str->string);
    EXIT;
    return NULL;
}

bmpl_object* get_object_type(bmpl_object_types type)
{
    switch (type)
    {
        case NUM_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"ЧИСЛО", ARENA), ARENA);
            break;
        case STR_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"СТРОКА", ARENA), ARENA);
            break;
        case BOOL_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"ЛОГИЧЕСКИЙ", ARENA), ARENA);
            break;
        case LIST_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"СПИСОК", ARENA), ARENA);
            break;
        case FUNC_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"ФУНКЦИЯ", ARENA), ARENA);
            break;
        case VOID_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"ПУСТОТА", ARENA), ARENA);
            break;
        case INSTRYCTION_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"ИНСТРУКЦИЯ", ARENA), ARENA);
            break;
        case RETURN_OBJ:
            return new_object(STR_OBJ, bmpl_string_from_str(L"ВОЗВРАЩЕНИЕ", ARENA), ARENA);
            break;
        default:
            wprintf(L"Интерпретатор: невозможно получить тип объекта, неизвестный тип <%d>\n", type);
            EXIT;
            break;
    }
    return NULL;
}