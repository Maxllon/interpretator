#include "parcer.h"

static tk_node *tk_list = NULL;

struct Expretion *create_empty_expr(expr_kind kind)
{
    struct Expretion *expr = malloc(sizeof(struct Expretion));
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
        exit(1);
        break;
    }
    return expr;
}

struct Seque *create_empty_seque(void)
{
    struct Seque *seque = malloc(sizeof(struct Seque));

    seque->expretions.len = 0;
    seque->expretions.size = 100;
    seque->expretions.expr = calloc(100, sizeof(struct Expretion *));

    return seque;
}
struct Func *create_empty_func(void)
{
    struct Func *func = malloc(sizeof(struct Func));
    func->name = NULL;
    func->body = NULL;

    func->arguments.len = 0;
    func->arguments.size = 100;
    func->arguments.expr = calloc(100, sizeof(struct Expretion *));

    return func;
}
struct Call *create_empty_call(void)
{
    struct Call *call = malloc(sizeof(struct Call));
    call->name = NULL;

    call->arguments.len = 0;
    call->arguments.size = 100;
    call->arguments.expr = calloc(100, sizeof(struct Expretion *));

    return call;
}
struct IF *create_empty_if(void)
{
    struct IF *If = malloc(sizeof(struct IF));
    If->cond = NULL;
    If->els = NULL;
    If->then = NULL;
    return If;
}
struct Number *create_empty_number(void)
{
    struct Number *number = malloc(sizeof(struct Number));
    number->value = NULL;
    return number;
}
struct String *create_empty_string(void)
{
    struct String *str = malloc(sizeof(struct String));
    str->value = NULL;
    return str;
}
struct Boolean *create_empty_boolean(void)
{
    struct Boolean *boolean = malloc(sizeof(struct Boolean));
    boolean->value = NULL;
    return boolean;
}
struct Variable *create_empty_variable(void)
{
    struct Variable *var = malloc(sizeof(struct Variable));
    var->name = NULL;
    return var;
}
struct Assign *create_empty_assign(void)
{
    struct Assign *assign = malloc(sizeof(struct Assign));
    assign->right = NULL;
    assign->var = NULL;
    return assign;
}
struct Array *create_empty_array(void)
{
    struct Array *array = malloc(sizeof(struct Array));

    array->expretions.len = 0;
    array->expretions.size = 100;
    array->expretions.expr = calloc(100, sizeof(struct Expretion *));

    return array;
}
struct Binary *create_empty_binary(void)
{
    struct Binary *bin = malloc(sizeof(struct Binary));
    bin->left = NULL;
    bin->op = NULL;
    bin->right = NULL;
    return bin;
}

struct Index* create_empty_index(void)
{
    struct Index* index = malloc(sizeof(struct Index));
    index->index = NULL;
    index->destination = NULL;
    return index;
}

struct Return* create_empty_return(void)
{
    struct Return* return_t = malloc(sizeof(struct Return));
    return_t->value = NULL;
    return return_t;
}

struct While* create_empty_while(void)
{
    struct While* while_t = malloc(sizeof(struct While));
    while_t->body = NULL;
    while_t->cond = NULL;
    return while_t;
}

struct Foreach* create_empty_foreach(void)
{
    struct Foreach* foreach = malloc(sizeof(struct Foreach));
    foreach->body = NULL;
    foreach->list = NULL;
    foreach->var = NULL;
    return foreach;
}

void delete_expr(struct Expretion *expr)
{
    if (expr == NULL)
        return;
    switch (expr->kind)
    {
    case SEQUE_EXPR:
        delete_seque(expr->seque);
        break;
    case FUNC_EXPR:
        delete_func(expr->func);
        break;
    case CALL_EXPR:
        delete_call(expr->call);
        break;
    case IF_EXPR:
        delete_if(expr->If);
        break;
    case NUMBER_EXPR:
        delete_number(expr->number);
        break;
    case STRING_EXPR:
        delete_string(expr->string);
        break;
    case BOOLEAN_EXPR:
        delete_boolean(expr->boolean);
        break;
    case VARIABLE_EXPR:
        delete_variable(expr->variable);
        break;
    case ASSIGN_EXPR:
        delete_assign(expr->assign);
        break;
    case BINARY_EXPR:
        delete_binary(expr->binary);
        break;
    case ARRAY_EXPR:
        delete_array(expr->array);
        break;
    case INDEX_EXPR:
        delete_index(expr->index);
        break;
    case VOID_EXPR:
        break;
    case RETURN_EXPR:
        delete_return(expr->return_t);
        break;
    case WHILE_EXPR:
        delete_while(expr->while_t);
        break;
    case FOREACH_EXPR:
        delete_foreach(expr->foreach);
        break;
    default:
        wprintf(L"Ошибка: некорректный тип выражения\n");
        system("pause");
        exit(1);
        break;
    }
    bm_free(expr);
}
void delete_seque(struct Seque *seque)
{
    for (size_t i = 0; i < seque->expretions.len; ++i)
    {
        delete_expr(*(seque->expretions.expr + i));
    }
    bm_free(seque->expretions.expr);
    bm_free(seque);
}
void delete_func(struct Func *func)
{
    for (size_t i = 0; i < func->arguments.len; ++i)
    {
        delete_expr(*(func->arguments.expr + i));
    }
    bm_free(func->name);
    delete_expr(func->body);
    bm_free(func->arguments.expr);
    bm_free(func);
}
void delete_call(struct Call *call)
{
    for (size_t i = 0; i < call->arguments.len; ++i)
    {
        delete_expr(*(call->arguments.expr + i));
    }
    bm_free(call->name);
    bm_free(call->arguments.expr);
    bm_free(call);
}
void delete_if(struct IF *If)
{
    delete_expr(If->cond);
    delete_expr(If->then);
    delete_expr(If->els);
    bm_free(If);
}
void delete_number(struct Number *num)
{
    bm_free(num->value);
    bm_free(num);
}
void delete_string(struct String *str)
{
    bm_free(str->value);
    bm_free(str);
}
void delete_boolean(struct Boolean *boolean)
{
    bm_free(boolean->value);
    bm_free(boolean);
}
void delete_variable(struct Variable *var)
{
    bm_free(var->name);
    bm_free(var);
}
void delete_assign(struct Assign *assign)
{
    delete_expr(assign->var);
    delete_expr(assign->right);
    bm_free(assign);
}
void delete_array(struct Array *array)
{
    for (size_t i = 0; i < array->expretions.len; ++i)
    {
        delete_expr(*(array->expretions.expr + i));
    }
    bm_free(array->expretions.expr);
    bm_free(array);
}
void delete_binary(struct Binary *bin)
{
    delete_expr(bin->left);
    delete_expr(bin->right);
    bm_free(bin->op);
    bm_free(bin);
}

void delete_index(struct Index* index)
{
    delete_expr(index->index);
    delete_expr(index->destination);
    bm_free(index);
}

void delete_return(struct Return* return_t)
{
    delete_expr(return_t->value);
    bm_free(return_t);
}

void delete_while(struct While* while_t)
{
    delete_expr(while_t->body);
    delete_expr(while_t->cond);
    bm_free(while_t);
}

void delete_foreach(struct Foreach* foreach)
{
    delete_expr(foreach->body);
    delete_expr(foreach->var);
    delete_expr(foreach->list);
    bm_free(foreach);
}

struct Expretion *at(expretion_Vector* expretions, size_t index)
{
    if (index > expretions->len)
    {
        printf("Error: index is out of range!\n");
        system("pause");
        exit(1);
    }
    return *(expretions->expr + index);
}

void push_back(expretion_Vector* expretions, struct Expretion* expr)
{
    expretions->len++;
    if(expretions->size < expretions->len)
    {
        expretions->expr = realloc(expretions->expr, expretions->size + 100);
    }
    *(expretions->expr + expretions->len - 1) = expr;
}

int skip(wchar *str)
{
    if(tk_list->kind == END)
    {
        wprintf(L"Ошибка: упущен символ <%ls> в позиции <%d><%d>\n", str, tk_list->pos.x, tk_list->pos.y);
        system("pause");
        exit(1);
        return 1;
    }
    if (wcscmp(tk_list->value, str) == 0)
    {
        tk_list = tk_list->next;
        return 0;
    }
    wprintf(L"Ошибка: упущен символ <%ls> в позиции <%d><%d>\n", str, tk_list->pos.x, tk_list->pos.y);
    system("pause");
    exit(1);
    return 1;
}

struct Expretion *parce(tk_node *main)
{
    tk_list = main;
    tk_list = tk_list->next;
    struct Expretion *expr = create_empty_expr(SEQUE_EXPR);
    while (tk_list->kind != END)
    {
        push_back(&expr->seque->expretions, parce_expr());
        skip(L";");
    }
    if(out_tree) out_expretion(expr, 0);
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
                expr->boolean->value = malloc(wcslen(tk_list->value)*2+2);
                wcscpy(expr->boolean->value, tk_list->value);
                break;
            }
            if(wcscmp(tk_list->value, L"доселе") == 0) return parce_while();
            if(wcscmp(tk_list->value, L"бери") == 0) return parce_foreach();
            break;
        case NUMBER:
            expr = create_empty_expr(NUMBER_EXPR);
            expr->number->value = malloc(wcslen(tk_list->value)*2+2);
            wcscpy(expr->number->value, tk_list->value);
            break;
        case STRING:
            expr = create_empty_expr(STRING_EXPR);
            expr->string->value = malloc(wcslen(tk_list->value)*2+2);
            wcscpy(expr->string->value, tk_list->value);
            break;
        case VARIABLE:
            if(tk_list->next->kind != END) if (wcscmp(tk_list->next->value, L"(") == 0) return parce_call();
            expr = create_empty_expr(VARIABLE_EXPR);
            expr->variable->name = malloc(wcslen(tk_list->value)*2+2);
            wcscpy(expr->variable->name, tk_list->value);
            break;
        default:
            break;
    }
    tk_list = tk_list->next;
    return expr;
}

static op_priority operators[] = 
{
    {L"=", 1},

    {L"або", 2},

    {L"и", 3},

    {L"==", 4},
    {L"<=", 4},
    {L">=", 4},
    {L">", 4},
    {L"<", 4},
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
        if(wcscmp(op, operators[i].operator)) return operators[i].priority;
    }
    wprintf(L"Cant find this op: %ls\n", op);
    system("pause");
    exit(1);
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
            expr->binary->op = malloc(wcslen(op)+2);
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
            system("pause");
            exit(1);
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
        push_back(&expr->seque->expretions, parce_expr());
        skip(L";");
    }
    tk_list = tk_list->next;
    return expr;
}

struct Expretion* parce_func(void)
{
    tk_list = tk_list->next;
    struct Expretion* expr = create_empty_expr(FUNC_EXPR);

    expr->func->name = malloc(wcslen(tk_list->value)*2+2);
    wcscpy(expr->func->name, tk_list->value);
    skip(expr->func->name);

    skip(L"(");
    while(wcscmp(tk_list->value, L")") != 0)
    {
        push_back(&expr->func->arguments, parce_expr());
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
    expr->call->name = malloc(wcslen(tk_list->value)*2+2);
    wcscpy(expr->call->name, tk_list->value);

    tk_list = tk_list->next;
    skip(L"(");
    while(wcscmp(tk_list->value, L")") != 0)
    {
        push_back(&expr->call->arguments, parce_expr());
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
        push_back(&expr->array->expretions, parce_expr());
        if(wcscmp(tk_list->value, L"}") != 0) skip(L",");
    }
    skip(L"}");
    return expr;
}

void out_seque(struct Seque* seque, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: последовательность\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    for(size_t i = 0; i < seque->expretions.len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(at(&seque->expretions, i), indent+INDENT);
    }
    wprintf(L"%ls}\n", str);

    free(str);
}

void out_func(struct Func* func, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: функция\n", str);
    wprintf(L"%lsИмя: %ls\n", str, func->name);

    wprintf(L"%lsАргументы:\n", str);
    wprintf(L"%ls(\n", str);
    for(size_t i = 0; i < func->arguments.len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(at(&func->arguments, i), indent+INDENT);
    }
    wprintf(L"%ls)\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expretion(func->body, indent+INDENT);
    wprintf(L"%ls}\n", str);

    free(str);
}

void out_call(struct Call* call, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: вызов функции\n", str);
    wprintf(L"%lsИмя: %ls\n", str, call->name);

    wprintf(L"%lsАргументы:\n", str);
    wprintf(L"%ls(\n", str);
    for(size_t i = 0; i < call->arguments.len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(at(&call->arguments, i), indent+INDENT);
    }
    wprintf(L"%ls)\n", str);

    free(str);
}

void out_index(struct Index* index, size_t indent)
{
    wchar* str = malloc(2*indent+2);
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

    free(str);
}

void out_binary(struct Binary* bin, size_t indent)
{
    wchar* str = malloc(2*indent+2);
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
    wprintf(L"%ls}\n", str);
    free(str);
}

void out_number(struct Number* num, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: число\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, num->value);

    free(str);
}

void out_variable(struct Variable* var, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: переменнная\n", str);
    wprintf(L"%lsИмя: %ls\n", str, var->name);

    free(str);
}

void out_string(struct String* string, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: строка\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, string->value);

    free(str);
}

void out_if(struct IF* If, size_t indent)
{
    wchar* str = malloc(2*indent+2);
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
    wchar* str = malloc(2*indent+2);
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

    free(str);
}
void out_void(size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: пустота\n", str);

    free(str);  
}
void out_boolean(struct Boolean* boolean, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: логический\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, boolean->value);

    free(str); 
}

void out_while(struct While* while_t, size_t indent)
{
    wchar* str = malloc(2*indent+2);
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

    free(str);  
}

void out_foreach(struct Foreach* foreach, size_t indent)
{
    wchar* str = malloc(2*indent+2);
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

    free(str);  
}

void out_array(struct Array* array, size_t indent)
{
    wchar* str = malloc(2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';

    wprintf(L"%lsТип: список\n", str);

    wprintf(L"%lsЭлементы:\n", str);
    wprintf(L"%ls[\n", str);
    for(size_t i = 0; i < array->expretions.len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expretion(at(&array->expretions, i), indent+INDENT);
    }
    wprintf(L"%ls]\n", str);


    free(str);
}