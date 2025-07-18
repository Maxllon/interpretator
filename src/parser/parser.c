#include<expression.h>
#include<globals.h>

static link_node* tk_list;

#define get_token ((token*)tk_list->value)

static int
skip(int type, int subtype)
{
    if(get_token->subtype == subtype && (int)get_token->type == type) tk_list = tk_list->next;
    else
    {
        LAST_ERROR.pos_start = get_token->_pos;
        LAST_ERROR.type = SYNTAX_ERROR;
        return -1;
    }
    return ALL_GOOD;
}

static expression*
create_empty_expr(expr_kind type);

static expression*
parse_atom();

static expression*
parse_expr();

static expression*
parse_num();

static int
get_prior(op_type type);

static expression*
mb_method(expression* expr);

static expression*
mb_index(expression* expr);

static expression*
mb_call(expression* expr);

static expression*
mb_binary(expression* left, int prior);

static expression*
parse_atom();

static expression*
parse_un();

static expression*
parse_expr();

static expression*
parse_seque();

static expression*
parse_if();

static expression*
parse_variable();

static expression*
parse_class();

static expression*
parse_foreach();

static expression*
parse_while();

static expression*
parse_func();

static expression*
parse_instryct();

static expression*
parse_return();

static expression*
parse_string();

static expression*
parse_list();

static expression*
parse_bool();


static expression*
create_empty_expr(expr_kind type)
{
    expression* expr = arena_alloc(sizeof(expression));
    if(expr == NULL) return NULL;
    expr->type = type;
    expr->_pos = get_token->_pos;
    switch(type)
    {
        case EXPR_BIN:
            bin_expr* bn = arena_alloc(sizeof(bin_expr));
            if(bn == NULL) return NULL;
            expr->expr.binary = bn;
            break;
        case EXPR_UN:
            un_expr* un = arena_alloc(sizeof(un_expr));
            if(un == NULL) return NULL;
            expr->expr.unary = un;
            break;
        case EXPR_NUM:
            num_expr* num = arena_alloc(sizeof(num_expr));
            if(num == NULL) return NULL;
            expr->expr.number = num;
            break;
        case EXPR_SEQUE:
            seque_expr* seque = arena_alloc(sizeof(seque_expr));
            if(seque == NULL) return NULL;
            seque->list = NULL;
            expr->expr.seque = seque;
            break;
        case EXPR_IF:
            if_expr* _if = arena_alloc(sizeof(if_expr));
            if(_if == NULL) return NULL;
            _if->cond = NULL;
            _if->then = NULL;
            _if->_else = NULL;
            expr->expr._if = _if;
            break;
        case EXPR_VAR:
            var_expr* var = arena_alloc(sizeof(var_expr));
            if(var == NULL) return NULL;
            var->value = NULL;
            expr->expr.var = var;
            break;
        case EXPR_INDEX:
            index_expr* index = arena_alloc(sizeof(index_expr));
            if(index == NULL) return NULL;
            expr->expr.index = index;
            break;
        case EXPR_CLASS:
            class_expr* class = arena_alloc(sizeof(class_expr));
            if(class == NULL) return NULL;
            expr->expr.class = class;
            break;
        case EXPR_FOREACH:
            foreach_expr* _for = arena_alloc(sizeof(foreach_expr));
            if(_for == NULL) return NULL;
            expr->expr._for = _for;
            break;
        case EXPR_WHILE:
            while_expr* _while = arena_alloc(sizeof(while_expr));
            expr->expr._while = _while;
            break;
        case EXPR_LIST:
            list_expr* list = arena_alloc(sizeof(list_expr));
            expr->expr.list = list;
            break;
        case EXPR_CALL:
            call_expr* call = arena_alloc(sizeof(call_expr));
            expr->expr.call = call;
            break;
        case EXPR_FUNC:
            func_expr* func = arena_alloc(sizeof(func_expr));
            expr->expr.func = func;
            break;
        case EXPR_INSTRYCT:
            instryct_expr* instryct = arena_alloc(sizeof(instryct_expr));
            expr->expr.instryct = instryct;
            break;
        case EXPR_RETURN:
            return_expr* _return = arena_alloc(sizeof(return_expr));
            expr->expr._return = _return;
            break;
        case EXPR_METHOD:
            method_expr* method = arena_alloc(sizeof(method_expr));
            expr->expr.method = method;
            break;
        case EXPR_STRING:
            string_expr* _string = arena_alloc(sizeof(string_expr));
            expr->expr._string = _string;
            break;
        case EXPR_BOOL:
            bool_expr* _bool = arena_alloc(sizeof(bool_expr));
            expr->expr._bool = _bool;
            break;
        default:    
            printf("Парсер: Неизвестный тип выражения: %d\n", type);
            arena_destroy();
            exit(1);
    }
    return expr;
}

static expression*
parse_list()
{
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_LIST);
    if(res == NULL) return NULL;
    res->expr.list->list = NULL;
    while(get_token->type != KEY_SEPARATOR || get_token->subtype != KEY_SQUARE_CLOSE)
    {
        expression* temp = parse_expr();
        if(temp == NULL) return NULL;
        res->expr.list->list = link_push_back(res->expr.list->list, link_new(temp));
        if(res->expr.list->list == NULL) return NULL;
        if(get_token->type != KEY_SEPARATOR || get_token->subtype != KEY_SQUARE_CLOSE)
        {
            if(skip(KEY_SEPARATOR, KEY_COMMA) != ALL_GOOD) return NULL;
        }
    }
    res->expr.list->list = link_go_start(res->expr.list->list);
    if(skip(KEY_SEPARATOR, KEY_SQUARE_CLOSE) != ALL_GOOD) return NULL;
    return res;
}

static expression*
parse_string()
{
    expression* res = create_empty_expr(EXPR_STRING);
    if(res == NULL) return NULL;
    res->expr._string->value = get_token->value;
    tk_list = tk_list->next;
    return res;
}

static expression*
parse_return()
{
    expression* res = create_empty_expr(EXPR_RETURN);
    if(res == NULL) return NULL;
    tk_list = tk_list->next;
    if((res->expr._return->expr = parse_expr()) == NULL) return NULL;
    return res;
}

static expression*
parse_instryct()
{
    expression* res = create_empty_expr(EXPR_INSTRYCT);
    if(res == NULL) return NULL;
    res->expr.instryct->type = get_token->subtype;
    tk_list = tk_list->next;
    return res;
}

static expression*
parse_func()
{
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_FUNC);
    if(res == NULL) return NULL;
    if((res->expr.func->call = parse_expr()) == NULL) return NULL;
    if(skip(KEY_SEPARATOR, KEY_COLON) != ALL_GOOD) return NULL;
    if((res->expr.func->body = parse_expr()) == NULL) return NULL;
    return res;
}

static expression*
parse_while()
{
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_WHILE);
    if((res->expr._while->cond = parse_expr()) == NULL) return NULL;
    if(skip(KEY_SEPARATOR, KEY_COLON) != ALL_GOOD) return NULL;
    if((res->expr._while->body = parse_expr()) == NULL) return NULL;
    return res;
}

static expression*
parse_foreach()
{
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_FOREACH);
    if(res == NULL) return NULL;
    if((res->expr._for->iter = parse_expr())==NULL) return NULL;
    if(skip(KEY_KEYWORD, KEY_IN) != ALL_GOOD) return NULL;
    if((res->expr._for->list = parse_expr())==NULL) return NULL;
    if(skip(KEY_SEPARATOR, KEY_COLON) != ALL_GOOD) return NULL;
    if((res->expr._for->body = parse_expr())==NULL) return NULL;
    return res;
}

static expression*
parse_class()
{
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_CLASS);
    if((res->expr.class->name = parse_atom()) == NULL) return NULL;
    if(res->expr.class->name->type != EXPR_VAR)
    {
        LAST_ERROR.pos_start = get_token->_pos;
        LAST_ERROR.type = INCORRECT_NAME_OBJECT;
        return NULL;
    }
    res->expr.class->derive = NULL;
    if(get_token->type == KEY_SEPARATOR && get_token->subtype == KEY_PARENTHESIS_OPEN)
    {
        tk_list = tk_list->next;
        if((res->expr.class->derive = parse_atom()) == NULL) return NULL;
        if(res->expr.class->derive->type != EXPR_VAR)
        {
            LAST_ERROR.pos_start = get_token->_pos;
            LAST_ERROR.type = INCORRECT_NAME_OBJECT;
            return NULL;
        }
        if(skip(KEY_SEPARATOR, KEY_PARENTHESIS_CLOSE) != ALL_GOOD) return NULL;
    }
    if(skip(KEY_SEPARATOR, KEY_COLON) != ALL_GOOD) return NULL;
    if((res->expr.class->body = parse_expr()) == NULL) return NULL;
    return res;
}

static expression*
mb_call(expression* expr)
{
    if(expr == NULL) return NULL;
    if(get_token->type != KEY_SEPARATOR) return expr;
    if(get_token->subtype != KEY_PARENTHESIS_OPEN) return expr;
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_CALL);
    if(res == NULL) return NULL;
    res->expr.call->dest = expr;
    res->expr.call->argc = NULL;
    while(get_token->type != KEY_SEPARATOR || get_token->subtype != KEY_PARENTHESIS_CLOSE)
    {
        expression* temp = parse_expr();
        if(temp == NULL) return NULL;
        res->expr.call->argc = link_push_back(res->expr.call->argc, link_new(temp));
        if(res->expr.call->argc == NULL) return NULL;
        if(get_token->type != KEY_SEPARATOR || get_token->subtype != KEY_PARENTHESIS_CLOSE)
        {
            if(skip(KEY_SEPARATOR, KEY_COMMA) != ALL_GOOD) return NULL;
        }
    }
    res->expr.call->argc = link_go_start(res->expr.call->argc);
    if(skip(KEY_SEPARATOR, KEY_PARENTHESIS_CLOSE) != ALL_GOOD) return NULL;
    return mb_method(mb_index(mb_call(res)));
}

static expression*
mb_method(expression* expr)
{
    if(expr == NULL) return NULL;
    if(get_token->type != KEY_SEPARATOR) return expr;
    if(get_token->subtype != KEY_DOT) return expr;
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_METHOD);
    if(res == NULL) return NULL;
    res->expr.method->dest = expr;
    res->expr.method->call = parse_expr();
    return mb_method(mb_index(mb_call(res)));
}

static expression*
mb_index(expression* expr)
{
    if(expr == NULL) return NULL;
    if(get_token->type != KEY_SEPARATOR) return expr;
    if(get_token->subtype != KEY_SQUARE_OPEN) return expr;
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_INDEX);
    if(res == NULL) return NULL;
    if((res->expr.index->index = parse_expr()) == NULL) return NULL;
    res->expr.index->dest = expr;
    if(skip(KEY_SEPARATOR, KEY_SQUARE_CLOSE) != ALL_GOOD) return NULL;
    return mb_method(mb_index(mb_call(res)));
}

static expression*
parse_variable()
{
    expression* res = create_empty_expr(EXPR_VAR);
    if(res == NULL) return NULL;
    res->expr.var->value = get_token->value;
    tk_list = tk_list->next;
    return res;
}

static expression*
parse_if()
{
    tk_list = tk_list->next;
    expression* res = create_empty_expr(EXPR_IF);
    if(res == NULL) return NULL;
    if((res->expr._if->cond = parse_expr()) == NULL) return NULL;;
    if(skip(KEY_SEPARATOR, KEY_COLON) != ALL_GOOD) return NULL;
    if((res->expr._if->then = parse_expr()) == NULL) return NULL;
    res->expr._if->_else = NULL;
    if(get_token->subtype != KEY_ELSE) return res;
    tk_list = tk_list->next;
    if((res->expr._if->_else = parse_expr()) == NULL) return NULL;
    return res;
}


static expression*
parse_num()
{
    expression* res = create_empty_expr(EXPR_NUM);
    if(res == NULL) return NULL;
    res->expr.number->value = get_token->value;
    tk_list = tk_list->next;
    return res;
}

static int
get_prior(op_type type)
{
    switch(type)
    {
        case KEY_ASSIGN: return 1;
        case KEY_EQUAL: 
        case KEY_NOT_EQUAL:
        case KEY_GREATER:
        case KEY_LESS:
        case KEY_GREATER_EQUAL:
        case KEY_LESS_EQUAL: return 2;
        case KEY_OR: return 3;
        case KEY_AND: return 4;
        case KEY_XOR: return 5;
        case KEY_XOR_BIT: return 6;
        case KEY_OR_BIT: return 7;
        case KEY_AND_BIT: return 8;
        case KEY_PLUS:
        case KEY_MINUS: return 9;
        case KEY_MULT:
        case KEY_DIV:
        case KEY_FLOOR_DIV:
        case KEY_MOD: return 10;
        case KEY_POW: return 11;
        default:
            printf("Парсер: Неизвестный оператор: %d\n", type);
            arena_destroy();
            exit(1);
    }
    return -1;
}

static expression*
mb_binary(expression* left, int prior)
{
    if(left == NULL) return NULL;
    if(get_token->type != KEY_OP) return left;
    int his_prior = get_prior(get_token->subtype);
    if(his_prior > prior)
    {
        expression* expr = create_empty_expr(EXPR_BIN);
        if(expr == NULL) return NULL;
        string* op = get_token->value;
        op_type type = get_token->subtype;
        tk_list = tk_list->next;
        expression* right = mb_binary(mb_method(mb_index(mb_call(parse_atom()))), his_prior);
        if(right == NULL) return NULL;
        expr->expr.binary->left = left;
        expr->expr.binary->right = right;
        expr->expr.binary->op = op;
        expr->expr.binary->type = type;
        return mb_binary(expr, prior);
    }
    return left;
}

static expression*
parse_seque()
{
    skip(KEY_KEYWORD, KEY_START_BLOCK);
    expression* res = create_empty_expr(EXPR_SEQUE);
    if(res == NULL) return NULL;
    while(get_token->type != KEY_END && get_token->subtype != KEY_END_BLOCK)
    {
        expression* expr = parse_expr();
        if(expr == NULL) return NULL;
        res->expr.seque->list = link_push_back(res->expr.seque->list, link_new(expr));
        if(res->expr.seque->list == NULL) return NULL;
        if(skip(KEY_SEPARATOR, KEY_SEMICOLON) != ALL_GOOD) return NULL;
    }
    if(skip(KEY_KEYWORD, KEY_END_BLOCK) != ALL_GOOD) return NULL;
    if(res->expr.seque->list != NULL) res->expr.seque->list = link_go_start(res->expr.seque->list);
    return res;
}

static expression*
parse_keyword()
{
    switch(get_token->subtype)
    {
        case KEY_START_BLOCK:
            return parse_seque();
        case KEY_IF:
            return parse_if();
        case KEY_CLASS:
            return parse_class();
        case KEY_FOREACH:
            return parse_foreach();
        case KEY_WHILE: 
            return parse_while();
        case KEY_FUNC:
            return parse_func();
        case KEY_RETURN:
            return parse_return();
    }
    printf("Неизвестное ключевое слово: %d\n", get_token->subtype);
    arena_destroy();
    exit(1);
    return NULL;
}

static expression*
parse_separator()
{
    switch(get_token->subtype)
    {
        case KEY_PARENTHESIS_OPEN:
            tk_list = tk_list->next;
            expression* res = parse_expr();
            if(res == NULL) return NULL;
            if(skip(KEY_SEPARATOR, KEY_PARENTHESIS_CLOSE) != ALL_GOOD) return NULL;
            return res;
        case KEY_SQUARE_OPEN:
            return parse_list();
    }
    printf("Неизвестный сепаратор: %d\n", get_token->subtype);
    arena_destroy();
    exit(1);
    return NULL;
}

static expression*
parse_bool()
{
    expression* res = create_empty_expr(EXPR_BOOL);
    if(!res) return NULL;
    if(get_token->subtype == KEY_TRUE) res->expr._bool->is_true = True;
    else res->expr._bool->is_true = False;
    res->expr._bool->value = get_token->value;
    tk_list = tk_list->next;
    return res;
}

static expression*
parse_atom()
{
    if(get_token->type == KEY_END)
    {
        LAST_ERROR.pos_start = get_token->_pos;
        LAST_ERROR.type = SYNTAX_ERROR;
        return NULL;
    }
    switch(get_token->type)
    {
        case KEY_OP:
            return parse_un();
        case KEY_NUMBER:
            return parse_num();
        case KEY_KEYWORD:
            return parse_keyword();
        case KEY_SEPARATOR:
            return parse_separator();
        case KEY_VARIABLE:
            return parse_variable();
        case KEY_INSTRYCTION:
            return parse_instryct();
        case KEY_STRING:
            return parse_string();
        case KEY_BOOL:
            return parse_bool();
        case KEY_START: case KEY_END: break;
    }
    LAST_ERROR.pos_start = get_token->_pos;
    LAST_ERROR.type = UNKNOWN_TOKEN;
    return NULL;
}

static expression*
parse_un()
{
    expression* res = create_empty_expr(EXPR_UN);
    if(res == NULL) return NULL;
    
    switch(get_token->subtype)
    {
        case KEY_MINUS:
        case KEY_PLUS:
        case KEY_NOT:
        case KEY_NOT_BIT:
            break;
        default:
            LAST_ERROR.pos_start = get_token->_pos;
            LAST_ERROR.type = UNKNOWN_UN;
            return NULL;
    }
    res->expr.unary->op = get_token->value;
    res->expr.unary->type = get_token->subtype;
    tk_list = tk_list->next;
    res->expr.unary->expr = parse_atom();
    if(res->expr.unary->expr == NULL) return NULL;
    return res;
}

static expression*
parse_expr()
{
    return mb_binary(mb_method(mb_call(mb_index(parse_atom()))), 0);
}

expression* 
parse(link_node* node)
{
    tk_list = node->next;
    expression* res = create_empty_expr(EXPR_SEQUE);
    if(res == NULL) return NULL;
    while(get_token->type != KEY_END)
    {
        expression* expr = parse_expr();
        if(expr == NULL) return NULL;
        res->expr.seque->list = link_push_back(res->expr.seque->list, link_new(expr));
        if(res->expr.seque->list == NULL) return NULL;
        if(skip(KEY_SEPARATOR, KEY_SEMICOLON) != ALL_GOOD) return NULL;
    }
    if(res->expr.seque->list != NULL) res->expr.seque->list = link_go_start(res->expr.seque->list);
    return res;
}

#define make_indent(indent) for(int i=0;i<indent;++i) putchar(' ')

static void
out_atom(expression* root, int indent);

static void
out_var(expression* root, int indent)
{
    make_indent(indent);
    printf("Переменная: ");
    str_out(root->expr.var->value);
    putchar('\n');
}

static void
out_bin(expression* root, int indent)
{
    make_indent(indent);
    printf("Оператор: ");
    str_out(root->expr.binary->op);
    putchar('\n');
    make_indent(indent);
    printf("Выражение слева:\n");
    out_atom(root->expr.binary->left, indent + BASE_INDENT);
    make_indent(indent);
    printf("Выражение справа:\n");
    out_atom(root->expr.binary->right, indent + BASE_INDENT);
}

static void
out_un(expression* root, int indent)
{
    make_indent(indent);
    printf("Оператор: ");
    str_out(root->expr.unary->op);
    putchar('\n');
    make_indent(indent);
    printf("Выражение:\n");
    out_atom(root->expr.unary->expr, indent + BASE_INDENT);
}

static void
out_num(expression* root, int indent)
{
    make_indent(indent);
    printf("Число: ");
    str_out(root->expr.number->value);
    putchar('\n');
}

static void
out_seque(expression* root, int indent)
{
    make_indent(indent);
    printf("нч:\n");
    link_node* temp = root->expr.seque->list;
    int i=1;
    while(temp != NULL)
    {
        make_indent(indent);
        printf("%d)\n", i);
        out_atom((expression*)temp->value, indent + BASE_INDENT);
        i++;
        temp = temp->next;
    }
    make_indent(indent);
    printf("кц.\n");
}

static void
out_if(expression* root, int indent)
{
    make_indent(indent);
    printf("Условие:\n");
    out_atom(root->expr._if->cond, indent + BASE_INDENT);
    make_indent(indent);
    printf("Выражение при \"Истина\":\n");
    out_atom(root->expr._if->then, indent + BASE_INDENT);
    if(root->expr._if->_else == NULL) return;
    make_indent(indent);
    printf("Выражение при \"Ложь\":\n");
    out_atom(root->expr._if->_else, indent + BASE_INDENT);
}

static void
out_index(expression* root, int indent)
{
    make_indent(indent);
    printf("Индекс:\n");
    out_atom(root->expr.index->index, indent + BASE_INDENT);
    make_indent(indent);
    printf("Из выражения:\n");
    out_atom(root->expr.index->dest, indent + BASE_INDENT);
}

static void
out_class(expression* root, int indent)
{
    make_indent(indent);
    printf("Имя объекта:\n");
    out_atom(root->expr.class->name, indent + BASE_INDENT);
    if(root->expr.class->derive != NULL)
    {
        make_indent(indent);
        printf("Унаследован от:\n");
        out_atom(root->expr.class->derive, indent + BASE_INDENT);
    }
    make_indent(indent);
    printf("Тело объекта:\n");
    out_atom(root->expr.class->body, indent + BASE_INDENT);
}

static void
out_foreach(expression* root, int indent)
{
    make_indent(indent);
    printf("Итератор:\n");
    out_atom(root->expr._for->iter, indent + BASE_INDENT);
    make_indent(indent);
    printf("Список:\n");
    out_atom(root->expr._for->list, indent + BASE_INDENT);
    make_indent(indent);
    printf("Тело:\n");
    out_atom(root->expr._for->body, indent + BASE_INDENT);
}

static void
out_while(expression* root, int indent)
{
    make_indent(indent);
    printf("Условие цикла:\n");
    out_atom(root->expr._while->cond, indent + BASE_INDENT);
    make_indent(indent);
    printf("Тело цикла:\n");
    out_atom(root->expr._while->body, indent + BASE_INDENT);
}

static void
out_call(expression* root, int indent)
{
    make_indent(indent);
    printf("Вызов из:\n");
    out_atom(root->expr.call->dest, indent + BASE_INDENT);
    make_indent(indent);
    printf("нч:\n");
    link_node* temp = root->expr.call->argc;
    int i=1;
    while(temp != NULL)
    {
        make_indent(indent);
        printf("%d)\n", i);
        out_atom((expression*)temp->value, indent + BASE_INDENT);
        i++;
        temp = temp->next;
    }
    make_indent(indent);
    printf("кц.\n");
}

static void
out_func(expression* root, int indent)
{
    make_indent(indent);
    printf("Функция:\n");
    out_atom(root->expr.func->call, indent + BASE_INDENT);
    make_indent(indent);
    printf("Тело функции:\n");
    out_atom(root->expr.func->body, indent + BASE_INDENT);
}

static void
out_instryct(expression* root, int indent)
{
    make_indent(indent);
    printf("Инструкция: ");
    switch(root->expr.instryct->type)
    {
        case KEY_BREAK:
            printf("Выход из функции");
            break;
        case KEY_CONTINUE:
            printf("Продолжить функцию");
            break;
        default:
            printf("UNTITLED");
            break;
    }
    putchar('\n');
}

static void
out_return(expression* root, int indent)
{
    make_indent(indent);
    printf("Вернуть:\n");
    out_atom(root->expr._return->expr, indent + BASE_INDENT);
}

static void
out_string(expression* root, int indent)
{
    make_indent(indent);
    printf("Строка: ");
    str_out(root->expr._string->value);
    putchar('\n');
}

static void
out_list(expression* root, int indent)
{
    make_indent(indent);
    printf("Список:\n");
    make_indent(indent);
    printf("нч:\n");
    link_node* temp = root->expr.list->list;
    int i=1;
    while(temp != NULL)
    {
        make_indent(indent);
        printf("%d)\n", i);
        out_atom((expression*)temp->value, indent + BASE_INDENT);
        i++;
        temp = temp->next;
    }
    make_indent(indent);
    printf("кц.\n");

}

static void
out_method(expression* root, int indent)
{
    make_indent(indent);
    printf("Метод:\n");
    out_atom(root->expr.method->call, indent + BASE_INDENT);
    make_indent(indent);
    printf("Из:\n");
    out_atom(root->expr.method->dest, indent + BASE_INDENT);
}

static void
out_bool(expression* root, int indent)
{
    make_indent(indent);
    printf("Логическое: ");
    str_out(root->expr._bool->value);
    putchar('\n');
}

static void 
out_atom(expression* root, int indent)
{
    switch(root->type)
    {
        case EXPR_BIN:
            out_bin(root, indent);
            break;
        case EXPR_UN:
            out_un(root, indent);
            break;
        case EXPR_NUM:
            out_num(root, indent);
            break;
        case EXPR_SEQUE:
            out_seque(root, indent);
            break;
        case EXPR_IF:
            out_if(root, indent);
            break;
        case EXPR_VAR:
            out_var(root, indent);
            break;
        case EXPR_INDEX:
            out_index(root, indent);
            break;
        case EXPR_CLASS:
            out_class(root, indent);
            break;
        case EXPR_FOREACH: 
            out_foreach(root, indent);
            break;
        case EXPR_WHILE:
            out_while(root, indent);
            break;
        case EXPR_CALL:
            out_call(root, indent);
            break;
        case EXPR_FUNC:
            out_func(root, indent);
            break;
        case EXPR_INSTRYCT:
            out_instryct(root, indent);
            break;
        case EXPR_RETURN:
            out_return(root, indent);
            break;
        case EXPR_STRING:
            out_string(root, indent);
            break;
        case EXPR_LIST:
            out_list(root, indent);
            break;
        case EXPR_METHOD:
            out_method(root, indent);
            break;
        case EXPR_BOOL:
            out_bool(root, indent);
            break;
        default:
            printf("Вывод: Неизвестный тип выражения: %d\n", root->type);
            arena_destroy();
            exit(1);
            break;
    }
}

void
out_AST(expression* root)
{
    out_atom(root, 0);
}

#undef make_indent