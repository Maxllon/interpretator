#include"interpretator.h"

static Arena* ARENA;
static module* CURR_MODULE;

void interpretate(Expression* expr, Arena* arena)
{
    if(expr == NULL || arena == NULL) return;
    ARENA = arena;
    CURR_MODULE = new_module(arena, NULL);
    std_funcs();
    interpretate_atom(expr);
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
        case INDEX_EXPR:
            res = interpretate_index(expr);
            break;
        case VOID_EXPR:
            res = new_object(VOID_OBJ, NULL, ARENA);
            break;
        case IF_EXPR:
            res = interpretate_if(expr);
            break;
        case WHILE_EXPR:
            res = interpretate_while(expr);
            break;
        case FOREACH_EXPR:
            res = interpretate_foreach(expr);
            break;
        case INSTRUCTION_EXPR:
            res = interpretate_instryction(expr);
            break;
        case FUNC_EXPR:
            res = interpretate_func(expr);
            break;
        case CALL_EXPR:
            res = interpretate_call(expr);
            break;
        case RETURN_EXPR:
            res = interpretate_return(expr);
            break;
        default:
            wprintf(L"Интерпретация: неизвестный тип выражения: %d\n", expr->kind);
            EXIT;
            break;
    }
    return res;
}

bmpl_object* interpretate_return(Expression* expr)
{
    return new_object(RETURN_OBJ, interpretate_atom(expr->_return->value), ARENA);
}

static void out(bmpl_object* obj)
{
    switch(obj->type)
    {
        case STR_OBJ:
            wprintf(L"%ls", obj->str->string);
            break;
        case NUM_OBJ:
            wprintf(L"%ls", str_from_big_num(obj->num, ARENA));
            break;
        case BOOL_OBJ:
            wchar* temp = L"Истина";
            if(!obj->_bool) temp = L"Ложь";
            wprintf(L"%ls", temp);
            break;
        case VOID_OBJ:
            wprintf(L"ПУСТОТА");
            break;
        case LIST_OBJ:
            wprintf(L"[");
            big_num* one = big_num_from_str(L"1", ARENA);
            bmpl_object* a;
            for(big_num* i = big_num_from_str(L"0", ARENA); compare_big(i, obj->root->sons) == -1; i = sum_big(i, one, ARENA))
            {
                a = dk_get_el(obj->root, i, ARENA);
                if(a->type == STR_OBJ)
                {
                    wprintf(L"\"");
                }
                out(a);
                if(a->type == STR_OBJ)
                {
                    wprintf(L"\"");
                }
                if(compare_big(i, sub_big(obj->root->sons, one, ARENA))) wprintf(L", ");
            }
            wprintf(L"]");
            break;
        default:
            wprintf(L"Интерпретатор: неподходящий тип для вывода \"%ls\"\n", get_object_type(obj->type)->str->string);
            EXIT;
    }
}

static bmpl_object* print(bm_vector* args)
{
    if(args->len != 1)
    {
        wprintf(L"Интерпретатор: функция \"печать\" ожидает %llu аргументов. Получено %llu аргументов\n", 2, args->len);
        EXIT;
    }
    bmpl_object* a = interpretate_atom(bm_vector_at(args, 0));
    out(a);
    return a;
}

void std_funcs()
{
    add_var(CURR_MODULE, new_variable(bmpl_string_from_str(L"печать", ARENA), new_object(FUNC_OBJ, &print, ARENA), ARENA), ARENA);
}
bmpl_object* interpretate_call(Expression* expr)
{
    bmpl_string* name = bmpl_string_from_str(expr->call->name, ARENA);

    variable* var = find_var(CURR_MODULE, name, ARENA);
    if(!var)
    {
        wprintf(L"Интерпретатор: нет функции с именем \"%ls\"\n", name->string);
        EXIT;
    }
    if(var->value->type != FUNC_OBJ)
    {
        wprintf(L"Интерпретатор: нет функции с именем \"%ls\"\n", name->string);
        EXIT;
    }
    if(var->value->func->is_std)
    {
        bmpl_object* (*restored_func_ptr)(bm_vector*) = (bmpl_object* (*)(bm_vector*))var->value->func;
        return restored_func_ptr(expr->call->arguments);
    }
    if(var->value->func->args->len != expr->call->arguments->len)
    {
        wprintf(L"Интерпретатор: функция \"%ls\" ожидает %llu аргументов. Получено %llu аргументов\n", name->string, var->value->func->args->len, expr->call->arguments->len);
        EXIT;
    }
    bmpl_string* arg_name;
    variable* arg_var;
    bmpl_object* obj;
    Expression* call_one;

    module* temp = CURR_MODULE;
    CURR_MODULE = new_module(ARENA, temp);

    for(size_t i = 0; i<var->value->func->args->len;++i)
    {
        call_one = bm_vector_at(expr->call->arguments, i);
        obj = get_object(interpretate_atom(call_one), ARENA);

        arg_name = bmpl_string_from_str(((Expression*)bm_vector_at(var->value->func->args, i))->variable->name, ARENA);
        arg_var = new_variable(arg_name, obj, ARENA);
        add_var(CURR_MODULE, arg_var, ARENA);
    }
    obj = interpretate_atom(var->value->func->body);
    if(obj->type == RETURN_OBJ) obj = obj->ret;
    CURR_MODULE = temp;
    return obj;
}

bmpl_object* interpretate_func(Expression* expr)
{

    func_object* fnc = arena_alloc(ARENA, sizeof(func_object));
    fnc->args = expr->func->arguments;
    fnc->body = expr->func->body;
    fnc->is_std = 0;
    bmpl_object* obj = new_object(FUNC_OBJ, fnc, ARENA);

    bmpl_string* name = bmpl_string_from_str(expr->func->name, ARENA);
    variable* var = find_var(CURR_MODULE, name, ARENA);
    if(!var)
    {
        var = new_variable(name, obj, ARENA);
        add_var(CURR_MODULE, var, ARENA);
    }
    else var->value = obj;

    return obj;
}
bmpl_object* interpretate_instryction(Expression* expr)
{
    instruction_types* type = arena_alloc(ARENA, sizeof(instruction_types));
    if(wcscmp(L"завершить", expr->instruction->name) == 0) *type = BREAK;
    else if(wcscmp(L"продолжить", expr->instruction->name) == 0) *type = CONTINUE;
    else
    {
        wprintf(L"Интерпретация: неизвестная инструкция \"%ls\"\n", expr->instruction->name);
        EXIT;
    }
    return new_object(INSTRYCTION_OBJ, type, ARENA);
}
bmpl_object* interpretate_while(Expression* expr)
{
    bmpl_object* obj;
    bmpl_object* cond;
    while(1)
    {
        cond = interpretate_atom(expr->_while->cond);
        if(cond->type != BOOL_OBJ)
        {
            wprintf(L"Интерпретация: условие цикла должно быть типа \"%ls\"\n", get_object_type(BOOL_OBJ)->str->string);
            EXIT;
        }
        if(!cond->_bool) break;
        obj = interpretate_atom(expr->_while->body);
        if(obj->type == INSTRYCTION_OBJ)
        {
            if(obj->instr == BREAK) return new_object(VOID_OBJ, NULL, ARENA);
            if(obj->instr == CONTINUE) obj = new_object(VOID_OBJ, NULL, ARENA);
        }
        if(obj->type == RETURN_OBJ) return obj;
    }
    return obj;
}
bmpl_object* interpretate_foreach(Expression* expr)
{
    bmpl_object* obj = new_object(VOID_OBJ, NULL, ARENA);;
    big_num* one = big_num_from_str(L"1", ARENA);
    bmpl_object* arr = interpretate_atom(expr->foreach->list);
    if(expr->foreach->var->kind != VARIABLE_EXPR)
    {
        wprintf(L"Интерпретация: в цикле for-each ожидалась переменная\n");
        EXIT;
    }
    bmpl_string* name = bmpl_string_from_str(expr->foreach->var->variable->name, ARENA);
    variable* var = find_var(CURR_MODULE, name, ARENA);
    if(!var)
    {
        var = new_variable(name, obj, ARENA);
        add_var(CURR_MODULE, var, ARENA);
    }
    else var->value = obj;
    if(arr->type != LIST_OBJ)
    {
        wprintf(L"Интерпретация: тип \"%ls\" не итерируется\n", get_object_type(arr->type)->str->string);
        EXIT;
    }
    for(big_num* i = big_num_from_str(L"0", ARENA); compare_big(i, arr->root->sons) == -1; i = sum_big(i, one, ARENA))
    {
        var->value = dk_get_el(arr->root, i, ARENA);
        obj = interpretate_atom(expr->foreach->body);
        if(obj->type == INSTRYCTION_OBJ)
        {
            if(obj->instr == BREAK) return new_object(VOID_OBJ, NULL, ARENA);
            if(obj->instr == CONTINUE) obj = new_object(VOID_OBJ, NULL, ARENA);
        }
        if(obj->type == RETURN_OBJ) return obj;
    }
    return obj;
}
bmpl_object* interpretate_if(Expression* expr)
{
    bmpl_object* condition = interpretate_atom(expr->_if->cond);
    if(condition->type != BOOL_OBJ)
    {
        wprintf(L"Интерпретатор: условие в если должно быть \"%ls\", а представлено \"%ls\"\n", get_object_type(BOOL_OBJ)->str->string, get_object_type(condition->type)->str->string);
        EXIT;
    }
    if(condition->_bool)
    {
        return interpretate_atom(expr->_if->then);
    }
    else if(expr->_if->_else)
    {
        return interpretate_atom(expr->_if->_else);
    }
    return new_object(VOID_OBJ, NULL, ARENA);
}
bmpl_object* interpretate_seque(Expression* expr)
{
    bmpl_object* obj;
    for(size_t i = 0; (Expression*)bm_vector_at(expr->seque->expressions, i) != NULL; ++i)
    {
        obj = interpretate_atom((Expression*)bm_vector_at(expr->seque->expressions, i));
        if(obj->type == INSTRYCTION_OBJ) return obj;
        if(obj->type == RETURN_OBJ) return obj;
    }
    return obj;
}
bmpl_object* interpretate_index(Expression* expr)
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
    big_num* index;
    switch(dest->type)
    {
        case LIST_OBJ:
            index = num->num;
            if(index->is_negative)
            {
                index = sum_big(dest->root->sons, index, ARENA);
            }
            if(index->is_negative || compare_big(index, dest->root->sons) != -1)
            {
                wprintf(L"Интерпретация: индекс списка вне его размера\n");
                EXIT;
            }
            return dk_get_el(dest->root, index, ARENA);
        case STR_OBJ:
            index = num->num;
            big_num* size = big_num_from_ull(dest->str->size, ARENA);
            if(index->is_negative)
            {
                index = sum_big(size, index, ARENA);
            }
            if(index->is_negative || compare_big(index, size) != -1)
            {
                wprintf(L"Интерпретация: индекс строки вне ее размера\n");
                EXIT;
            }
            return new_object(STR_OBJ, slice(dest->str, index, sum_big(index, big_num_from_str(L"1", ARENA), ARENA),ARENA), ARENA);

        default:
            wprintf(L"Обращаться по индексу можно только к спискам и строкам\n");
            EXIT;
    }
}
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

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\"\n", op, get_object_type(BOOL_OBJ)->str->string);
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

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\"\n", op, get_object_type(NUM_OBJ)->str->string);
    EXIT;
}
bmpl_object* interpretate_unary(Expression* expr)
{
    wchar* op = expr->unary->op;
    bmpl_object* value = interpretate_atom(expr->unary->value);
    if(value->type == BOOL_OBJ) return un_bool(op, value->_bool);
    if(value->type == NUM_OBJ) return un_num(op, value->num);

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\"\n", op, get_object_type(value->type)->str->string);
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
    if(expr->binary->left->kind == INDEX_EXPR)
    {
        bmpl_object* dest = interpretate_atom(expr->binary->left);
        bmpl_object* obj = copy_object(interpretate_atom(expr->binary->right), ARENA);
        *dest = *obj;
        return dest;
    }
    wprintf(L"Интерпретация: неподходящий тип левого выражения в присваивании\n");
    EXIT;
    return NULL;
    
}

bmpl_object* bin_list_list(wchar* op, dk_node* left, dk_node* right)
{
    if(wcscmp(op, L"+") == 0) return new_object(LIST_OBJ, dk_merge(left, right, ARENA), ARENA);

    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(LIST_OBJ)->str->string, op, get_object_type(LIST_OBJ)->str->string);
    EXIT;
}
bmpl_object* bin_list_num(wchar* op, bmpl_object* left, big_num* right)
{
    if(wcscmp(op, L"*") == 0)
    {
        bmpl_object* res = new_object(LIST_OBJ, NULL, ARENA);
        big_num* one = big_num_from_str(L"1", ARENA);
        for(; !right->is_negative; right = sub_big(right, one, ARENA))
        {
            bmpl_object* obj = copy_object(left, ARENA);
            res->root = dk_merge(res->root, obj->root, ARENA);
        }
        return res;
    }
    wprintf(L"Интерпретатор: Неизвестная операция: \"%ls\" \"%ls\" \"%ls\"\n", get_object_type(LIST_OBJ)->str->string, op, get_object_type(NUM_OBJ)->str->string);
    EXIT;
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
    if(left->type == LIST_OBJ && right->type == LIST_OBJ) return bin_list_list(op, copy_object(left, ARENA)->root, copy_object(right, ARENA)->root);
    if((left->type == NUM_OBJ || right->type == NUM_OBJ) && (left->type == LIST_OBJ || right->type == LIST_OBJ))
    {
        if(left->type == NUM_OBJ)
        {
            bmpl_object* temp = left;
            left = right;
            right = temp;
        }
        return bin_list_num(op, left, copy_object(right, ARENA)->num);
    }
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