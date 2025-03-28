#include"parser.h"

static tk_node* tk_list = NULL;
static Arena* ARENA = NULL;

Expression* create_empty_expr(expr_kind kind)
{
    Expression* expr = arena_alloc(ARENA, sizeof(Expression));
    expr->kind = kind;
    switch(kind) 
    {
        case SEQUE_EXPR:
            expr->seque = create_empty_seque();
            break;
        case CALL_EXPR:
            expr->call = create_empty_call();
            break;
        case IF_EXPR:
            expr->_if = create_empty_if();
            break;        
        case VARIABLE_EXPR:
            expr->variable = create_empty_variable();
            break;
        case ARRAY_EXPR:
            expr->array = create_empty_array();
            break;
        case BOOLEAN_EXPR:
            expr->boolean = create_empty_boolean();
            break;
        case STRING_EXPR:
            expr->string = create_empty_string();
            break;
        case NUMBER_EXPR:
            expr->number = create_empty_number();
            break;
        case INDEX_EXPR:
            expr->index = create_empty_index();
            break;
        case BINARY_EXPR:
            expr->binary = create_empty_binary();
            break;
        case UNARY_EXPR:
            expr->unary = create_empty_unary();
            break;
        case WHILE_EXPR:
            expr->_while = create_empty_while();
            break;
        case FOREACH_EXPR:
            expr->foreach = create_empty_foreach();
            break;
        case FUNC_EXPR:
            expr->func = create_empty_func();
            break;
        case RETURN_EXPR:
            expr->_return = create_empty_return();
            break;
        case INSTRUCTION_EXPR:
            expr->instruction = create_empty_instruction();
            break;
        case VOID_EXPR:
            break;
        default:
            wprintf(L"Парсинг: некорректный тип выражения <%d><%d>\n", tk_list->pos.x, tk_list->pos.y);
            EXIT;
            break;
    }
    return expr;
}

Seque* create_empty_seque()
{
    Seque* seque = arena_alloc(ARENA, sizeof(Seque));
    seque->expressions = bm_vector_create(ARENA);
    return seque;
}
Call* create_empty_call()
{
    Call* call = arena_alloc(ARENA, sizeof(Call));
    call->name = NULL;
    call->arguments = bm_vector_create(ARENA);
    return call;
}
If* create_empty_if()
{
    If* _if = arena_alloc(ARENA, sizeof(If));
    _if->cond = NULL;
    _if->_else = NULL;
    _if->then = NULL;
    return _if;
}
Variable* create_empty_variable()
{
    Variable* var = arena_alloc(ARENA, sizeof(Variable));
    var->name = NULL;
    return var;
}
Array* create_empty_array()
{
    Array* array = arena_alloc(ARENA, sizeof(Array));
    array->expressions = bm_vector_create(ARENA);
    return array;
}
Boolean* create_empty_boolean()
{
    Boolean* boolean = arena_alloc(ARENA, sizeof(Boolean));
    boolean->value = NULL;
    return boolean;
}
String* create_empty_string()
{
    String* str = arena_alloc(ARENA, sizeof(String));
    str->value = NULL;
    return str;
}
Number* create_empty_number()
{
    Number* number = arena_alloc(ARENA, sizeof(Number));
    number->value = NULL;
    return number;
}
Index* create_empty_index()
{
    Index* index = arena_alloc(ARENA, sizeof(Index));
    index->index = NULL;
    index->destination = NULL;
    return index;
}
Binary* create_empty_binary()
{
    Binary* bin = arena_alloc(ARENA, sizeof(Binary));
    bin->left = NULL;
    bin->op = NULL;
    bin->right = NULL;
    return bin;
}
Unary* create_empty_unary()
{
    Unary* un= arena_alloc(ARENA, sizeof(Unary));
    un->op = NULL;
    un->value = NULL;
    return un;
}
While* create_empty_while()
{
    While* _while = arena_alloc(ARENA, sizeof(While));
    _while->body = NULL;
    _while->cond = NULL;
    return _while;
}
Foreach* create_empty_foreach()
{
    Foreach* foreach = arena_alloc(ARENA, sizeof(Foreach));
    foreach->body = NULL;
    foreach->list = NULL;
    foreach->var = NULL;
    return foreach;
}
Func* create_empty_func()
{
    Func* func = arena_alloc(ARENA, sizeof(Func));
    func->name = NULL;
    func->body = NULL;
    func->arguments = bm_vector_create(ARENA);
    return func;
}
Return* create_empty_return()
{
    Return* _return = arena_alloc(ARENA, sizeof(Return));
    _return->value = NULL;
    return _return;
}
Instruction* create_empty_instruction()
{
    Instruction* instruct = arena_alloc(ARENA, sizeof(Instruction));
    instruct->name = NULL;
    return instruct;
}

static const op_priority PRIORITY[] =
{
    {L"=", 1},
    
    {L"ИЛИ", 2},

    {L"И", 3},

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
    
    {L"^", 7},
    {NULL, 0}
};

size_t find_priority(const wchar* op)
{
    for(size_t i = 0; PRIORITY[i].operator; ++i)
    {
        if(wcscmp(PRIORITY[i].operator, op) == 0)
        {
            return PRIORITY[i].priority;
        }
    }
    wprintf(L"Парсинг: некорректный оператор <%d><%d>\n", tk_list->pos.x, tk_list->pos.y);
    EXIT;
    return 0;
}
static void skip(wchar *str)
{
    if(tk_list->kind == END)
    {
        wprintf(L"Ошибка: упущен символ <%ls> в позиции <%d><%d>\n", str, tk_list->pos.x, tk_list->pos.y);
        EXIT;
        return;
    }
    if (wcscmp(tk_list->value, str) == 0)
    {
        tk_list = tk_list->next;
        return;
    }
    wprintf(L"Ошибка: упущен символ <%ls> в позиции <%d><%d>\n", str, tk_list->pos.x, tk_list->pos.y);
    EXIT;
}
Expression* parse(tk_node* main, Arena* arena)
{
    ARENA = arena;
    tk_list = main;
    tk_list = tk_list->next;
    Expression* expr = create_empty_expr(SEQUE_EXPR);
    while (tk_list->kind != END)
    {
        bm_vector_push(expr->seque->expressions, parse_expr());
        skip(L";");
    }
    if(out_tree && expr->array->expressions->len > 0) out_expression(expr, 0);
    return expr;
}

Expression* parse_expr()
{
    return mb_binary(mb_index(parse_atom()), 0);
}

Expression* parse_atom()
{
    if(tk_list->kind == END)
    {
        wprintf(L"Парсинг: нечего парсить(\n");
        EXIT;
    }
    Expression* expr;
    if(wcscmp(L"(", tk_list->value) == 0)
    {
        tk_list = tk_list->next;
        expr = parse_expr();
        skip(L")");
        return expr;
    }
    if(wcscmp(L"{", tk_list->value) == 0) return parse_array();
    switch(tk_list->kind)
    {
        case KEYWORD:
            if(wcscmp(tk_list->value, L"если") == 0) return parse_if();
            if(wcscmp(tk_list->value, L"пока") == 0) return parse_while();
            if(wcscmp(tk_list->value, L"для") == 0) return parse_foreach();
            if(wcscmp(tk_list->value, L"функ") == 0) return parse_func();
            if(wcscmp(tk_list->value, L"вернуть") == 0) return parse_return();
            if(wcscmp(tk_list->value, L"завершить") == 0) return parse_instruction();
            if(wcscmp(tk_list->value, L"продолжить") == 0) return parse_instruction();
            if(wcscmp(tk_list->value, L"НЕ") == 0) return parse_unary();
            if(wcscmp(tk_list->value, L"-") == 0) return parse_unary();
            if(wcscmp(tk_list->value, L"Истина") == 0) return parse_boolean();
            if(wcscmp(tk_list->value, L"Ложь") == 0) return parse_boolean();
            if(wcscmp(tk_list->value, L"нч") == 0) return parse_seque();
            if(wcscmp(tk_list->value, L"ПУСТОТА") == 0)
            {
                tk_list=tk_list->next;
                return create_empty_expr(VOID_EXPR);
            }
            wprintf(L"Парсинг: Неизвестное ключевое слово: %ls\n", tk_list->value);
            EXIT;
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
            if(tk_list->next->kind != END) if (wcscmp(tk_list->next->value, L"(") == 0) return parse_call();
            expr = create_empty_expr(VARIABLE_EXPR);
            expr->variable->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
            wcscpy(expr->variable->name, tk_list->value);
            break;
        default:
            wprintf(L"Парсинг: неизвестный тип токена: %d\n", tk_list->kind);
            EXIT;
            break;
    }
    tk_list = tk_list->next;
    return expr;
}
Expression* mb_binary(Expression* left, size_t priority)
{
    wchar* op = tk_list->value;
    if(tk_list->kind == BINARY)
    {
        size_t his_prior = find_priority(tk_list->value);
        if(his_prior > priority)
        {
            tk_list = tk_list->next;
            Expression* right = mb_binary(mb_index(parse_atom()), his_prior);
            Expression* expr = create_empty_expr(BINARY_EXPR);
            expr->binary->left = left;
            expr->binary->right = right;
            expr->binary->op = arena_alloc(ARENA, wcslen(op)*2 + 2);
            wcscpy(expr->binary->op, op);
            return mb_binary(expr, priority);
        }
    }
    return left;
}
Expression* mb_index(Expression* expr)
{
    if(tk_list->kind == SPECIAL)
    {
    if(wcscmp(tk_list->value, L"[") == 0)
    {
        tk_list = tk_list->next;

        Expression* index = create_empty_expr(INDEX_EXPR);
        index->index->destination = expr;
        index->index->index = parse_expr();
        skip(L"]");
        return mb_index(index);
    }
    }
    return expr;
}

Expression* parse_seque()
{
    tk_list = tk_list->next;
    Expression* expr = create_empty_expr(SEQUE_EXPR);

    while (wcscmp(tk_list->value, L"кц") != 0)
    {
        bm_vector_push(expr->seque->expressions, parse_expr());
        skip(L";");
        if(tk_list->kind == END)
        {
            wprintf(L"Парсинг: нет парного кц\n");
            EXIT;
        }
    }
    skip(L"кц");
    return expr;
}
Expression* parse_call()
{
    Expression* expr = create_empty_expr(CALL_EXPR);
    expr->call->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->call->name, tk_list->value);

    tk_list = tk_list->next;
    skip(L"(");
    while(wcscmp(tk_list->value, L")") != 0)
    {
        bm_vector_push(expr->call->arguments, parse_expr());
        if(tk_list->kind == END)
        {
            wprintf(L"Парсинг: нет парной скобки )\n");
            EXIT;
        }
        if(wcscmp(tk_list->value, L")") != 0) skip(L",");
    }
    skip(L")");
    return expr;
}

Expression* parse_if()
{
    tk_list = tk_list->next;
    Expression* expr = create_empty_expr(IF_EXPR);
    expr->_if->cond = parse_expr();
    skip(L":");
    expr->_if->then = parse_expr();
    if(wcscmp(tk_list->value, L"иначе") == 0)
    {
        tk_list = tk_list->next;
        skip(L":");
        expr->_if->_else = parse_expr();
    }

    else expr->_if->_else = NULL;
    return expr;
}

Expression* parse_array()
{
    tk_list = tk_list->next;

    Expression* expr = create_empty_expr(ARRAY_EXPR);

    while(wcscmp(tk_list->value, L"}") != 0)
    {
        bm_vector_push(expr->array->expressions, parse_expr());
        if(tk_list->kind == END)
        {
            wprintf(L"Парсинг: нет парной скобки }\n");
            EXIT;
        }
        if(wcscmp(tk_list->value, L"}") != 0) skip(L",");
    }
    skip(L"}");
    return expr;
}

Expression* parse_boolean()
{
    Expression* expr = create_empty_expr(BOOLEAN_EXPR);
    expr->boolean->value = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->boolean->value, tk_list->value);
    tk_list = tk_list->next;
    return expr;
}

Expression* parse_unary()
{
    Expression* expr = create_empty_expr(UNARY_EXPR);
    expr->unary->op = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->unary->op, tk_list->value);
    tk_list=tk_list->next;
    expr->unary->value = parse_expr();
    return expr;
}

Expression* parse_while()
{
    tk_list = tk_list->next;

    Expression* expr = create_empty_expr(WHILE_EXPR);
    expr->_while->cond = parse_expr();
    skip(L":");
    expr->_while->body = parse_expr();
    return expr;
}

Expression* parse_foreach()
{
    tk_list = tk_list->next;

    Expression* expr = create_empty_expr(FOREACH_EXPR);
    expr->foreach->var = parse_expr();
    skip(L"из");
    expr->foreach->list = parse_expr();
    skip(L":");
    expr->foreach->body = parse_expr();
    return expr;
}

Expression* parse_func()
{
    tk_list = tk_list->next;
    Expression* expr = create_empty_expr(FUNC_EXPR);

    expr->func->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->func->name, tk_list->value);
    skip(expr->func->name);

    skip(L"(");
    while(wcscmp(tk_list->value, L")") != 0)
    {
        bm_vector_push(expr->func->arguments, parse_expr());
        if(tk_list->kind == END)
        {
            wprintf(L"Парсинг: нет парной скобки )\n");
            EXIT;
        }
        if(wcscmp(tk_list->value, L")") != 0) skip(L",");
    }
    skip(L")");
    skip(L":");
    expr->func->body = parse_expr();
    return expr;
}

Expression* parse_return()
{
    tk_list = tk_list->next;
    Expression* expr = create_empty_expr(RETURN_EXPR);
    expr->_return->value = parse_expr();
    return expr;
}

Expression* parse_instruction()
{
    Expression* expr = create_empty_expr(INSTRUCTION_EXPR);
    expr->instruction->name = arena_alloc(ARENA, wcslen(tk_list->value)*2+2);
    wcscpy(expr->instruction->name, tk_list->value);
    tk_list=tk_list->next;
    return expr;
}
void out_expression(Expression* expr, size_t indent)
{
    switch(expr->kind)
    {
        case VOID_EXPR:
            out_void(indent);
            break;
        case SEQUE_EXPR:
            out_seque(expr->seque, indent);
            break;
        case CALL_EXPR:
            out_call(expr->call, indent);
            break;
        case IF_EXPR:
            out_if(expr->_if, indent);
            break;
        case VARIABLE_EXPR:
            out_variable(expr->variable, indent);
            break;
        case ARRAY_EXPR:
            out_array(expr->array, indent);
            break;
        case BOOLEAN_EXPR:
            out_boolean(expr->boolean, indent);
            break;
        case STRING_EXPR:
            out_string(expr->string, indent);
            break;
        case NUMBER_EXPR:
            out_number(expr->number, indent);
            break;
        case INDEX_EXPR:
            out_index(expr->index, indent);
            break;
        case BINARY_EXPR:
            out_binary(expr->binary, indent);
            break;
        case UNARY_EXPR:
            out_unary(expr->unary, indent);
            break;
        case WHILE_EXPR:
            out_while(expr->_while, indent);
            break;
        case FOREACH_EXPR:
            out_foreach(expr->foreach, indent);
            break;
        case FUNC_EXPR:
            out_func(expr->func, indent);
            break;
        case RETURN_EXPR:
            out_return(expr->_return, indent);
            break;
        case INSTRUCTION_EXPR:
            out_instruction(expr->instruction, indent);
            break;
        default:
            wprintf(L"Парсинг: неизвестный тип выражения\n");
            break;
    }
}

static wchar* make_indent(size_t indent)
{
    wchar* str = arena_alloc(ARENA, 2*indent+2);
    for(size_t i = 0; i < indent; ++i)
    {
        *(str+i) = L' ';
    }
    *(str+indent) = L'\0';
    return str;
}


void out_void(size_t indent)
{
    wchar* str = make_indent(indent);
    wprintf(L"%lsТип: ПУСТОТА\n", str);
    wprintf(L"%lsЗначение: ПУСТОТА\n", str);
}

void out_seque(Seque* seque, size_t indent)
{
    wchar* str = make_indent(indent);
    wprintf(L"%lsТип: последовательность\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    for(size_t i = 0; i < seque->expressions->len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expression(bm_vector_at(seque->expressions, i), indent+INDENT);
    }
    wprintf(L"%ls}\n", str);
}

void out_call(Call* call, size_t indent)
{
    wchar* str = make_indent(indent);
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
        out_expression(bm_vector_at(call->arguments, i), indent+INDENT);
    }
    wprintf(L"%ls)\n", str);
}

void out_if(If* _if, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: ветвление\n", str);
    wprintf(L"%lsУсловие:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(_if->cond, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsВыражение при \"ИСТИНА\":\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(_if->then, indent+INDENT);
    wprintf(L"%ls}\n", str);

    if(_if->_else != NULL)
    {
    wprintf(L"%lsВыражение при \"ЛОЖЬ\":\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(_if->_else, indent+INDENT);
    wprintf(L"%ls}\n", str);
    }
}

void out_variable(Variable* var, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: переменнная\n", str);
    wprintf(L"%lsИмя: %ls\n", str, var->name);
}

void out_array(Array* array, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: список\n", str);

    wprintf(L"%lsЭлементы:\n", str);
    wprintf(L"%ls[\n", str);
    for(size_t i = 0; i < array->expressions->len; ++i)
    {
        for(size_t j = 0; j < INDENT; ++j)
        {
            wprintf(L" ");
        }
        wprintf(L"%ls<%d>\n", str, i+1);
        out_expression(bm_vector_at(array->expressions, i), indent+INDENT);
    }
    wprintf(L"%ls]\n", str);
}

void out_boolean(Boolean* boolean, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: логический\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, boolean->value);
}

void out_string(String* string, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: строка\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, string->value);
}

void out_number(Number* number, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: число\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, number->value);
}

void out_index(Index* index, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: доступ к элементу\n", str);

    wprintf(L"%lsОткуда:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(index->destination, indent+INDENT);
    wprintf(L"%ls{\n", str);

    wprintf(L"%lsИндекс:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(index->index, indent+INDENT);
    wprintf(L"%ls{\n", str);
}

void out_binary(Binary* binary, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: бинарный\n", str);
    wprintf(L"%lsОператор: %ls\n", str, binary->op);

    wprintf(L"%lsСлева:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(binary->left, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsСправа:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(binary->right, indent+INDENT);
    wprintf(L"%ls}\n", str);;
}

void out_unary(Unary* unary, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: Унарный\n", str);
    wprintf(L"%lsОператор: %ls\n", str, unary->op);
    wprintf(L"%lsОперанд:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(unary->value, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_while(While* _while, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: цикл с условием\n", str);

    wprintf(L"%lsУсловие:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(_while->cond, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(_while->body, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_foreach(Foreach* foreach, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: цикл обхода коллекции\n", str);

    wprintf(L"%lsПеременная:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(foreach->var, indent+INDENT);
    wprintf(L"%ls}\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(foreach->body, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_func(Func* func, size_t indent)
{
    wchar* str = make_indent(indent);

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
        out_expression(bm_vector_at(func->arguments, i), indent+INDENT);
    }
    wprintf(L"%ls)\n", str);

    wprintf(L"%lsТело:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(func->body, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_return(Return* _return, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: возвращение\n", str);
    wprintf(L"%lsВозвращаемое значение:\n", str);
    wprintf(L"%ls{\n", str);
    out_expression(_return->value, indent+INDENT);
    wprintf(L"%ls}\n", str);
}

void out_instruction(Instruction* instruction, size_t indent)
{
    wchar* str = make_indent(indent);

    wprintf(L"%lsТип: Инструкция\n", str);
    wprintf(L"%lsЗначение: %ls\n", str, instruction->name);
}