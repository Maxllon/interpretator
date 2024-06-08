#include "parcer.h"

struct Expretion
{
    expr_kind kind;
    union expr
    {
        struct Seque *seque;
        struct Func *func;
        struct Call *call;
        struct IF *If;
        struct Number *number;
        struct String *string;
        struct Boolean *boolean;
        struct Variable *variable;
        struct Assign *assign;
        struct Array *array;
        struct Binary *binary;
    };
};
struct Seque
{
    expretion_Vector expretions;
};

struct Func
{
    wchar *name;

    expretion_Vector arguments;

    struct Expretion *body;
};

struct Call
{
    wchar *name;

    expretion_Vector arguments;
};

struct IF
{
    struct Expretion *cond;
    struct Expretion *then;
    struct Expretion *els;
};

struct Number
{
    wchar *value;
};

struct String
{
    wchar *value;
};

struct Boolean
{
    wchar *value;
};

struct Variable
{
    wchar *name;
};

struct Assign
{
    struct Expretion *var;
    struct Expretion *right;
};

struct Array
{
    expretion_Vector expretions;
};

struct Binary
{
    struct Expretion *right;
    struct Expretion *left;

    wchar *op;
};

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
    default:
        printf("Error: uncorrectly expretion kind!\n");
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
    default:
        printf("Error: uncorrectly expretion kind!\n");
        exit(1);
        break;
    }
    free(expr);
}
void delete_seque(struct Seque *seque)
{
    for (size_t i = 0; i < seque->expretions.len; ++i)
    {
        delete_expr(*(seque->expretions.expr + i));
    }
    free(seque->expretions.expr);
    free(seque);
}
void delete_func(struct Func *func)
{
    for (size_t i = 0; i < func->arguments.len; ++i)
    {
        delete_expr(*(func->arguments.expr + i));
    }
    free(func->name);
    delete_expr(func->body);
    free(func->arguments.expr);
    free(func);
}
void delete_call(struct Call *call)
{
    for (size_t i = 0; i < call->arguments.len; ++i)
    {
        delete_expr(*(call->arguments.expr + i));
    }
    free(call->name);
    free(call->arguments.expr);
    free(call);
}
void delete_if(struct IF *If)
{
    delete_expr(If->cond);
    delete_expr(If->then);
    delete_expr(If->els);
    free(If);
}
void delete_number(struct Number *num)
{
    free(num->value);
    free(num);
}
void delete_string(struct String *str)
{
    free(str->value);
    free(str);
}
void delete_boolean(struct Boolean *boolean)
{
    free(boolean->value);
    free(boolean);
}
void delete_variable(struct Variable *var)
{
    free(var->name);
    free(var);
}
void delete_assign(struct Assign *assign)
{
    delete_expr(assign->var);
    delete_expr(assign->right);
    free(assign);
}
void delete_array(struct Array *array)
{
    for (size_t i = 0; i < array->expretions.len; ++i)
    {
        delete_expr(*(array->expretions.expr + i));
    }
    free(array->expretions.expr);
    free(array);
}
void delete_binary(struct Binary *bin)
{
    delete_expr(bin->left);
    delete_expr(bin->right);
    free(bin->op);
    free(bin);
}

struct Expretion *at(expretion_Vector* expretions, size_t index)
{
    if (index > expretions->len)
    {
        printf("Error: index is out of range!\n");
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
    *(expretions->expr + expretions->len) = expr;
}

static tk_node *tk_list = NULL;

int skip(wchar *str)
{
    if (wcscmp(tk_list->value, str) == 0)
    {
        tk_list = tk_list->next;
        return 0;
    }
    return 1;
}

struct Expretion *parce(tk_node *main)
{
    tk_list = main;
    struct Expretion *expr = create_empty_expr(SEQUE_EXPR);

    while (tk_list->kind != END)
    {
        push_back(&expr->seque->expretions, parce_expr());
        skip(L";");
    }

    return expr;
}

struct Expretion *parce_expr(void)
{
    return parce_atom();
}

struct Expretion* parce_atom(void)
{
    
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
    
    {L"^", 7},
};

struct Expretion* mb_binary(struct Expretion* left, size_t priority)
{
    if(tk_list->kind == BINARY)
    {

    }
    return left;
}