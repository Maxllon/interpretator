#include"interpretator.h"

static Arena* ARENA;
static module* CURR_MODULE;

void interpretate(Expression* expr, Arena* arena)
{
    if(expr == NULL || arena == NULL) return;
    ARENA = arena;
    CURR_MODULE = new_module(arena, NULL);
    interpretate_atom(expr);
    variable* var = find_var(CURR_MODULE, bmpl_string_from_str(L"банан", ARENA));
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
        default:
            wprintf(L"Интерпретация: неизвестный тип выражения: %d\n", expr->kind);
            EXIT;
            break;
    }
    return res;
}

bmpl_object* interpretate_var(Expression* expr)
{
    variable* res = find_var(CURR_MODULE, bmpl_string_from_str(expr->variable->name, ARENA));
    if(!res)
    {
        wprintf(L"Интерпретация: не найдено переменной под именем: <%ls>\n", expr->variable->name);
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

    int* res = arena_alloc(ARENA, sizeof(int));
    *res = compare_big(left, right);
    if(wcscmp(op, L"==") == 0)
    {
        *res = (*res == 0) ? 1 : 0;
        return new_object(BOOL_OBJ, res, ARENA);
    }
    if(wcscmp(op, L">") == 0)
    {
        *res = (*res == 1) ? 1 : 0;
        return new_object(BOOL_OBJ, res, ARENA);
    }
    if(wcscmp(op, L"<") == 0)
    {
        *res = (*res == -1) ? 1 : 0;
        return new_object(BOOL_OBJ, res, ARENA);
    }
    if(wcscmp(op, L">=") == 0)
    {
        *res = (*res >= 0) ? 1 : 0;
        return new_object(BOOL_OBJ, res, ARENA);
    }
    if(wcscmp(op, L"<=") == 0)
    {
        *res = (*res <= 0) ? 1 : 0;
        return new_object(BOOL_OBJ, res, ARENA);
    }
    if(wcscmp(op, L"!=") == 0)
    {
        *res = (*res != 0) ? 1 : 0;
        return new_object(BOOL_OBJ, res, ARENA);
    }
    wprintf(L"Интерпретатор: Неизвестная операция: <%ls> <%ls> <%ls>\n", get_object_type(NUM_OBJ)->str->string, op, get_object_type(NUM_OBJ)->str->string);
    EXIT;
}

bmpl_object* assign(Expression* expr)
{
    if(expr->binary->left->kind == VARIABLE_EXPR)
    {
        bmpl_string* name = bmpl_string_from_str(expr->binary->left->variable->name, ARENA);
        bmpl_object* obj = copy_object(interpretate_atom(expr->binary->right), ARENA);
        variable* var = find_var(CURR_MODULE, name);
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
    /*if(left->type == STR_OBJ && right->type == STR_OBJ) return bin_str_str(op, left, right);
    if((left->type == NUM_OBJ || right->type == NUM_OBJ) && (left->type == STR_OBJ || right->type == STR_OBJ))
    {
        if(left->type == NUM_OBJ)
        {
            bmpl_object* temp = left;
            left = right;
            right = temp;
        }
        return bin_str_num(op, left, right);
    }*/
    wprintf(L"Интерпретатор: Неизвестная операция: <%ls> <%ls> <%ls>\n", get_object_type(left->type)->str->string, op, get_object_type(right->type)->str->string);
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
        default:
            wprintf(L"Интерпретатор: невозможно получить тип объекта, неизвестный тип <%d>\n", type);
            EXIT;
            break;
    }
    return NULL;
}