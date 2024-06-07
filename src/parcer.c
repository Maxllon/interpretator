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
    struct Expretion* values;
};

struct Binary
{
    struct Expretion* right;
    struct Expretion* left;

    wchar* op;
};

//дальше нужно много функций(...
