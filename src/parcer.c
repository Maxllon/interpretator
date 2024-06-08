#include"parcer.h"

struct Expretion
{
    expr_kind kind;
    union expr
    {
        struct Seque* seque;
        struct Func* func;
        struct Call* call;
        struct IF* If;
        struct Number* number;
        struct String* string;
        struct Boolean* boolean;
        struct Variable* variable;
        struct Assign* assign;
        struct Array* array;
        struct Binary* binary;
    };
};
struct Seque
{
    struct Expretion** expretions;
    size_t len;
};

struct Func
{
    wchar* name;

    struct Expretion** argv;
    size_t argc;

    struct Expretion* body;
};

struct Call
{
    wchar* name;

    struct Expretion** argv;
    size_t argc;
};

struct IF
{
    struct Expretion* cond;
    struct Expretion* then;
    struct Expretion* els;
};

struct Number
{
    wchar* value;
};

struct String
{
    wchar* value;
};

struct Boolean
{
    wchar* value;
};

struct Variable
{
    wchar* name;
};

struct Assign
{
    struct Expretion* var;
    struct Expretion* right;
};

struct Array
{
    size_t len;
    struct Expretion** values;
};

struct Binary
{
    struct Expretion* right;
    struct Expretion* left;

    wchar* op;
};

struct Expretion* create_empty_expr(expr_kind kind)
{
    struct Expretion* expr = malloc(sizeof(struct Expretion));
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

struct Seque* create_empty_seque(void)
{
    struct Seque* seque = malloc(sizeof(struct Seque));
    seque->expretions = NULL;
    seque->len = 0;
    return seque;
}
struct Func* create_empty_func(void)
{
    struct Func* func = malloc(sizeof(struct Func));
    func->name = NULL;
    func->argv = NULL;
    func->body = NULL;
    func->argc = 0;
    return func;
}
struct Call* create_empty_call(void)
{
    struct Call* call = malloc(sizeof(struct Call));
    call->name = NULL;
    call->argc = 0;
    call->argv = NULL;
    return call;
}
struct IF* create_empty_if(void)
{
    struct IF* If = malloc(sizeof(struct IF));
    If->cond = NULL;
    If->els = NULL;
    If->then = NULL;
    return If;
}
struct Number* create_empty_number(void)
{
    struct Number* number = malloc(sizeof(struct Number));
    number->value = NULL;
    return number;
}
struct String* create_empty_string(void)
{
    struct String* str = malloc(sizeof(struct String));
    str->value = NULL;
    return str;
}
struct Boolean* create_empty_boolean(void)
{
    struct Boolean* boolean = malloc(sizeof(struct Boolean));
    boolean->value = NULL;
    return boolean;
}
struct Variable* create_empty_variable(void)
{
    struct Variable* var = malloc(sizeof(struct Variable));
    var->name = NULL;
    return var;
}
struct Assign* create_empty_assign(void)
{
    struct Assign* assign = malloc(sizeof(struct Assign));
    assign->right = NULL;
    assign->var = NULL;
    return assign;
}
struct Array* create_empty_array(void)
{
    struct Array* array = malloc(sizeof(struct Array));
    array->len = 0;
    array->values = NULL;
    return array;
}
struct Binary* create_empty_binary(void)
{
    struct Binary* bin = malloc(sizeof(struct Binary));
    bin->left = NULL;
    bin->op = NULL;
    bin->right = NULL;
    return bin;
}

void delete_expr(struct Expretion* expr)
{
    if(expr == NULL) return;
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
void delete_seque(struct Seque* seque)
{
    for(size_t i = 0; i < seque->len; ++i)
    {
        delete_expr(*(seque->expretions + i));
    }
    free(seque->expretions);
    free(seque);
}
void delete_func(struct Func* func)
{
    for(size_t i = 0; i < func->argc; ++i)
    {
        delete_expr(*(func->argv + i));
    }
    free(func->name);
    delete_expr(func->body);
    free(func);
}
void delete_call(struct Call* call)
{
    for(size_t i = 0; i < call->argc; ++i)
    {
        delete_expr(*(call->argv + i));
    }
    free(call->name);
    free(call);
}
void delete_if(struct IF* If)
{
    delete_expr(If->cond);
    delete_expr(If->then);
    delete_expr(If->els);
    free(If);
}
void delete_number(struct Number* num)
{
    free(num->value);
    free(num);
}
void delete_string(struct String* str)
{
    free(str->value);
    free(str);
}
void delete_boolean(struct Boolean* boolean)
{
    free(boolean->value);
    free(boolean);
}
void delete_variable(struct Variable* var)
{
    free(var->name);
    free(var);
}
void delete_assign(struct Assign* assign)
{
    delete_expr(assign->var);
    delete_expr(assign->right);
    free(assign);
}
void delete_array(struct Array* array)
{
    for(size_t i = 0; i < array->len; ++i)
    {
        delete_expr(*(array->values + i));
    } 
    free(array);
}
void delete_binary(struct Binary* bin)
{
    delete_expr(bin->left);
    delete_expr(bin->right);
    free(bin->op);
    free(bin);
}