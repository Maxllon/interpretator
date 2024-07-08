#include "parcer.h"

static tk_node *tk_list = NULL;
static Arena* ARENA = NULL;

struct Expretion *create_empty_expr(expr_kind kind)
{
    struct Expretion *expr = arena_alloc(ARENA, sizeof(struct Expretion));
    expr->kind = kind;
    switch (kind)
    {
    case SEQUE_EXPR:
        expr->seque = create_empty_seque();
        break;
    case FUNC_EXPR:
        expr->func = create_empty_func();
        break;
    case CALL_EXPR:
        expr->call = create_empty_call();
        break;
    case IF_EXPR:
        expr->If = create_empty_if();
        break;
    case NUMBER_EXPR:
        expr->number = create_empty_number();
        break;
    case STRING_EXPR:
        expr->string = create_empty_string();
        break;
    case BOOLEAN_EXPR:
        expr->boolean = create_empty_boolean();
        break;
    case VARIABLE_EXPR:
        expr->variable = create_empty_variable();
        break;
    case ASSIGN_EXPR:
        expr->assign = create_empty_assign();
        break;
    case BINARY_EXPR:
        expr->binary = create_empty_binary();
        break;
    case ARRAY_EXPR:
        expr->array = create_empty_array();
        break;
    case INDEX_EXPR:
        expr->index = create_empty_index();
        break;
    case VOID_EXPR:
        break;
    case BREAK_EXPR:
        break;
    case RETURN_EXPR:
        expr->return_t = create_empty_return();
        break;
    case WHILE_EXPR:
        expr->while_t = create_empty_while();
        break;
    case FOREACH_EXPR:
        expr->foreach = create_empty_foreach();
        break;
    default:
        wprintf(L"Ошибка: некорректный тип выражения <%d><%d>\n", tk_list->pos.x, tk_list->pos.y);
        EXIT;
        break;
    }
    return expr;
}

struct Seque *create_empty_seque(void)
{
    struct Seque *seque = arena_alloc(ARENA, sizeof(struct Seque));

    seque->expretions = bm_vector_create(ARENA);

    return seque;
}
struct Func *create_empty_func(void)
{
    struct Func *func = arena_alloc(ARENA, sizeof(struct Func));
    func->name = NULL;
    func->body = NULL;

    func->arguments = bm_vector_create(ARENA);

    return func;
}
struct Call *create_empty_call(void)
{
    struct Call *call = arena_alloc(ARENA, sizeof(struct Call));
    call->name = NULL;

    call->arguments = bm_vector_create(ARENA);

    return call;
}
struct IF *create_empty_if(void)
{
    struct IF *If = arena_alloc(ARENA, sizeof(struct IF));
    If->cond = NULL;
    If->els = NULL;
    If->then = NULL;
    return If;
}
struct Number *create_empty_number(void)
{
    struct Number *number = arena_alloc(ARENA, sizeof(struct Number));
    number->value = NULL;
    return number;
}
struct String *create_empty_string(void)
{
    struct String *str = arena_alloc(ARENA, sizeof(struct String));
    str->value = NULL;
    return str;
}
struct Boolean *create_empty_boolean(void)
{
    struct Boolean *boolean = arena_alloc(ARENA, sizeof(struct Boolean));
    boolean->value = NULL;
    return boolean;
}
struct Variable *create_empty_variable(void)
{
    struct Variable *var = arena_alloc(ARENA, sizeof(struct Variable));
    var->name = NULL;
    return var;
}
struct Assign *create_empty_assign(void)
{
    struct Assign *assign = arena_alloc(ARENA, sizeof(struct Assign));
    assign->right = NULL;
    assign->var = NULL;
    return assign;
}
struct Array *create_empty_array(void)
{
    struct Array *array = arena_alloc(ARENA, sizeof(struct Array));

    array->expretions = bm_vector_create(ARENA);

    return array;
}
struct Binary *create_empty_binary(void)
{
    struct Binary *bin = arena_alloc(ARENA, sizeof(struct Binary));
    bin->left = NULL;
    bin->op = NULL;
    bin->right = NULL;
    return bin;
}

struct Index* create_empty_index(void)
{
    struct Index* index = arena_alloc(ARENA, sizeof(struct Index));
    index->index = NULL;
    index->destination = NULL;
    return index;
}

struct Return* create_empty_return(void)
{
    struct Return* return_t = arena_alloc(ARENA, sizeof(struct Return));
    return_t->value = NULL;
    return return_t;
}

struct While* create_empty_while(void)
{
    struct While* while_t = arena_alloc(ARENA, sizeof(struct While));
    while_t->body = NULL;
    while_t->cond = NULL;
    return while_t;
}

struct Foreach* create_empty_foreach(void)
{
    struct Foreach* foreach = arena_alloc(ARENA, sizeof(struct Foreach));
    foreach->body = NULL;
    foreach->list = NULL;
    foreach->var = NULL;
    return foreach;
}

int skip(wchar *str)
{
    if(tk_list->kind == END)
    {
        wprintf(L"Ошибка: упущен символ <%ls> в позиции <%d><%d>\n", str, tk_list->pos.x, tk_list->pos.y);
        EXIT;
        return 1;
    }
    if (wcscmp(tk_list->value, str) == 0)
    {
        tk_list = tk_list->next;
        return 0;
    }
    wprintf(L"Ошибка: упущен символ <%ls> в позиции <%d><%d>\n", str, tk_list->pos.x, tk_list->pos.y);
    EXIT;
    return 1;
}

struct Expretion *parce(tk_node *main, Arena* arena)
{
    ARENA = arena;
    tk_list = main;
    tk_list = tk_list->next;
    struct Expretion *expr = create_empty_expr(SEQUE_EXPR);
    while (tk_list->kind != END)
    {
        bm_vector_push(expr->seque->expretions, parce_expr());
        skip(L";");
    }
    if(out_tree && expr->array->expretions->len > 0) out_expretion(expr, 0);
    return expr;
}

struct Expretion *parce_expr(void)
{
    return mb_binary(mb_index(parce_atom()), 0);
}

struct Expretion* parce_atom(void)
{
    struct Expretion* expr;
    if(wcscmp(L"(", tk_list->value) == 0)
    {
        tk_list = tk_list->next;
        expr = parce_expr();
        skip(L")");
        return expr;
    }
    if(wcscmp(L"{", tk_list->value) == 0) return parce_array();
    switch(tk_list->kind)
    {
        case KEYWORD:
            if(wcscmp(tk_list->value, L"коли") == 0) return parce_if();
            if(wcscmp(tk_list->value, L"нч") == 0) return parce_seque();
            if(wcscmp(tk_list->value, L"служ") == 0) return parce_func();
            if(wcscmp(tk_list->value, L"НЕЧА") == 0)
            {
                tk_list = tk_list->next;
                return create_empty_expr(VOID_EXPR);
            }
            if(wcscmp(tk_list->value, L"обрати") == 0) return parce_return();
            if(wcscmp(tk_list->value, L"БЛЯДЬ") == 0 || wcscmp(tk_list->value, L"БЫЛЬ") == 0)
            {
                expr = create_empty_expr(BOOLEAN_EXPR);
                expr->boolean->value = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
                wcscpy(expr->boolean->value, tk_list->value);
                break;
            }
            if(wcscmp(tk_list->value, L"доселе") == 0) return parce_while();
            if(wcscmp(tk_list->value, L"бери") == 0) return parce_foreach();
            if(wcscmp(tk_list->value, L"рвать") == 0)
            {
                tk_list = tk_list->next;
                return create_empty_expr(BREAK_EXPR);
            }
            break;
        case NUMBER:
            expr = create_empty_expr(NUMBER_EXPR);
            expr->number->value = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
            wcscpy(expr->number->value, tk_list->value);
            break;
        case STRING:
            expr = create_empty_expr(STRING_EXPR);
            expr->string->value = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
            wcscpy(expr->string->value, tk_list->value);
            break;
        case VARIABLE:
            if(tk_list->next->kind != END) if (wcscmp(tk_list->next->value, L"(") == 0) return parce_call();
            expr = create_empty_expr(VARIABLE_EXPR);
            expr->variable->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
            wcscpy(expr->variable->name, tk_list->value);
            break;
        default:
            break;
    }
    tk_list = tk_list->next;
    return expr;
}

op_priority operators[] = 
{
    {L"=", 1},
    
    {L"або", 2},

    {L"и", 3},

    {L"==", 4},
    {L">", 4},
    {L"<", 4},
    {L"<=", 4},
    {L">=", 4},
    {L"!=", 4},

    {L"+", 5},
    {L"-", 5},
    
    {L"*", 6},
    {L"/", 6},
    {L"\%", 6},
    {L"//", 6},
    
    {L"^", 7}
};

size_t find_priority(const wchar* op)
{
    for(size_t i = 0; i < sizeof(operators)/sizeof(op_priority); ++i)
    {
        if(wcscmp(op, operators[i].operator) == 0) return operators[i].priority;
    }
    wprintf(L"Cant find this op: %ls\n", op);
    EXIT;
}

struct Expretion* mb_binary(struct Expretion* left, size_t priority)
{
    wchar* op = tk_list->value;
    if(tk_list->kind == BINARY)
    {
        size_t his_prior = find_priority(tk_list->value);
        if(his_prior > priority)
        {
            tk_list = tk_list->next;
            struct Expretion* right = mb_binary(mb_index(parce_atom()), his_prior);
            struct Expretion* expr = create_empty_expr(BINARY_EXPR);
            expr->binary->left = left;
            expr->binary->right = right;
            expr->binary->op = arena_alloc(ARENA, wcslen(op)*2 + 2);
            wcscpy(expr->binary->op, op);
            return mb_binary(expr, priority);
        }
    }
    return left;
}

void out_expretion(struct Expretion* expr, size_t indent)
{
    switch(expr->kind)
    {
        case FUNC_EXPR:
            out_func(expr->func, indent);
            break;
        case SEQUE_EXPR:
            out_seque(expr->seque, indent);
            break;
        case NUMBER_EXPR:
            out_number(expr->number, indent);
            break;
        case BINARY_EXPR:
            out_binary(expr->binary, indent);
            break;
        case VARIABLE_EXPR:
            out_variable(expr->variable, indent);
            break;
        case IF_EXPR:
            out_if(expr->If, indent);
            break;
        case STRING_EXPR:
            out_string(expr->string, indent);
            break;
        case CALL_EXPR:
            out_call(expr->call, indent);
            break;
        case INDEX_EXPR:
            out_index(expr->index, indent);
            break;
        case VOID_EXPR:
            out_void(indent);
            break;
        case BREAK_EXPR:
            out_break(indent);
            break;
        case RETURN_EXPR:
            out_return(expr->return_t, indent);
            break;
        case BOOLEAN_EXPR:
            out_boolean(expr->boolean, indent);
            break;
        case FOREACH_EXPR:
            out_foreach(expr->foreach, indent);
            break;
        case WHILE_EXPR:
            out_while(expr->while_t, indent);
            break;
        case ARRAY_EXPR:
            out_array(expr->array, indent);
            break;
        default:
            wprintf(L"Ошибка: некорректный тип выражения\n");
            EXIT;
            break;
    }
}

struct Expretion* mb_index(struct Expretion* expr)
{
    if(tk_list->kind == SPECIAL)
    {
    if(wcscmp(tk_list->value, L"[") == 0)
    {
        tk_list = tk_list->next;

        struct Expretion* index = create_empty_expr(INDEX_EXPR);
        index->index->destination = expr;
        index->index->index = parce_expr();
        skip(L"]");
        return mb_index(index);
    }
    }
    return expr;
}

struct Expretion* parce_if(void)
{
    tk_list = tk_list->next;
    struct Expretion* expr = create_empty_expr(IF_EXPR);
    expr->If->cond = parce_expr();
    skip(L":");
    expr->If->then = parce_expr();
    if(wcscmp(tk_list->value, L"инако") == 0)
    {
        tk_list = tk_list->next;
        skip(L":");
        expr->If->els = parce_expr();
    }

    else expr->If->els = NULL;
    return expr;
}

struct Expretion* parce_seque(void)
{
    tk_list = tk_list->next;
    struct Expretion* expr = create_empty_expr(SEQUE_EXPR);

    while (wcscmp(tk_list->value, L"кц") != 0)
    {
        bm_vector_push(expr->seque->expretions, parce_expr());
        skip(L";");
    }
    tk_list = tk_list->next;
    return expr;
}

struct Expretion* parce_func(void)
{
    tk_list = tk_list->next;
    struct Expretion* expr = create_empty_expr(FUNC_EXPR);

    expr->func->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->func->name, tk_list->value);
    skip(expr->func->name);

    skip(L"(");
    while(wcscmp(tk_list->value, L")") != 0)
    {
        bm_vector_push(expr->func->arguments, parce_expr());
        if(wcscmp(tk_list->value, L")") != 0) skip(L",");
    }
    skip(L")");
    skip(L":");
    expr->func->body = parce_expr();
    return expr;
}

struct Expretion* parce_call(void)
{
    struct Expretion* expr = create_empty_expr(CALL_EXPR);
    expr->call->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->call->name, tk_list->value);

    tk_list = tk_list->next;
    skip(L"(");
    while(wcscmp(tk_list->value, L")") != 0)
    {
        bm_vector_push(expr->call->arguments, parce_expr());
        if(wcscmp(tk_list->value, L")") != 0) skip(L",");
    }
    skip(L")");
    return expr;
}

struct Expretion* parce_return(void)
{
    tk_list = tk_list->next;
    struct Expretion* expr = create_empty_expr(RETURN_EXPR);
    expr->return_t->value = parce_expr();
    return expr;
}

struct Expretion* parce_while(void)
{
    tk_list = tk_list->next;

    struct Expretion* expr = create_empty_expr(WHILE_EXPR);
    expr->while_t->cond = parce_expr();
    skip(L":");
    expr->while_t->body = parce_expr();
    return expr;
}

struct Expretion* parce_foreach(void)
{
    tk_list = tk_list->next;

    struct Expretion* expr = create_empty_expr(FOREACH_EXPR);
    expr->foreach->var = parce_expr();
    skip(L"из");
    expr->foreach->list = parce_expr();
    skip(L":");
    expr->foreach->body = parce_expr();
    return expr;
}

struct Expretion* parce_array(void)
{
    tk_list = tk_list->next;

    struct Expretion* expr = create_empty_expr(ARRAY_EXPR);

    while(wcscmp(tk_list->value, L"}") != 0)
    {
        bm_vector_push(expr->array->expretions, parce_expr());
        if(wcscmp(tk_list->value, L"}") != 0) skip(L",");
    }
    skip(L"}");
    return expr;
}

void out_seque(struct Seque* seque, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: последовательность\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    for(size_t i = 0; i < seque->expretions->len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(bm_vector_at(seque->expretions, i), indent+INDENT);
    }
    wprintf(L"%ls}\n", str);

}

void out_func(struct Func* func, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: функция\n", str);
    wprintf(L"%lsИмя: %ls\n", str, func->name);

    wprintf(L"%lsАргументы:\n", str);
    wprintf(L"%ls(\n", str);
    for(size_t i = 0; i < func->arguments->len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(bm_vector_at(func->arguments, i), indent+INDENT);
    }
    wprintf(L"%ls)\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(func->body, indent+INDENT);
    wprintf(L"%ls}\n", str);

}

void out_call(struct Call* call, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: вызов функции\n", str);
    wprintf(L"%lsИмя: %ls\n", str, call->name);

    wprintf(L"%lsАргументы:\n", str);
    wprintf(L"%ls(\n", str);
    for(size_t i = 0; i < call->arguments->len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(bm_vector_at(call->arguments, i), indent+INDENT);
    }
    wprintf(L"%ls)\n", str);

}

void out_index(struct Index* index, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: доступ к элементу\n", str);

    wprintf(L"%lsОткуда:\n", str);
    wprintf(L"%ls{:\n", str);
    out_expretion(index->destination, indent+INDENT);
    wprintf(L"%ls{:\n", str);

    wprintf(L"%lsИндекс:\n", str);
    wprintf(L"%ls{:\n", str);
    out_expretion(index->index, indent+INDENT);
    wprintf(L"%ls{:\n", str);

}

void out_binary(struct Binary* bin, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: бинарный\n", str);
    wprintf(L"%lsОператор: %ls\n", str, bin->op);

    wprintf(L"%lsСлева:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(bin->left, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsСправа:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(bin->right, indent+INDENT);
    wprintf(L"%ls}\n", str);;
}

void out_number(struct Number* num, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: число\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, num->value);
}

void out_variable(struct Variable* var, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: переменнная\n", str);
    wprintf(L"%lsИмя: %ls\n", str, var->name);
}

void out_string(struct String* string, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: строка\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, string->value);
}

void out_if(struct IF* If, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: ветвление\n", str);
    wprintf(L"%lsУсловие:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(If->cond, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsВыражение при \"ИСТИНА\":\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(If->then, indent+INDENT);
    wprintf(L"%ls}\n", str);

    if(If->els != NULL)
    {
    wprintf(L"%lsВыражение при \"ЛОЖЬ\":\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(If->els, indent+INDENT);
    wprintf(L"%ls}\n", str);
    }

}
void out_return(struct Return* return_t, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: возвращение\n", str);
    wprintf(L"%lsВозвращаемое значение:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(return_t->value, indent+INDENT);
    wprintf(L"%ls}\n", str);
}
void out_void(size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: пустота\n", str);
}
void out_boolean(struct Boolean* boolean, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: логический\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, boolean->value);
}

void out_while(struct While* while_t, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: цикл пока\n", str);

    wprintf(L"%lsУсловие:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(while_t->cond, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(while_t->body, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_foreach(struct Foreach* foreach, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: цикл обхода коллекции\n", str);

    wprintf(L"%lsПеременная:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(foreach->var, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(foreach->body, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_array(struct Array* array, size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: список\n", str);

    wprintf(L"%lsЭлементы:\n", str);
    wprintf(L"%ls[\n", str);
    for(size_t i = 0; i < array->expretions->len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(bm_vector_at(array->expretions, i), indent+INDENT);
    }
    wprintf(L"%ls]\n", str);
}

void out_break(size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: выход из цикла\n", str);
}