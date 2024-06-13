#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include"includes.h"
#include"parcer.h"
#include"bm_vector.h"

typedef struct Environment Environment;
typedef struct Function Function;
typedef struct Data Data;
typedef struct bm_vector bm_vector;
typedef struct Expretion Expretion;

typedef struct Integer Integer;
typedef struct Str Str;
typedef struct List List;
typedef struct Float Float;

typedef enum
{
    INTEGER,
    FLOAT,
    STR,
    LIST
}DATA_TYPES;

struct Environment
{
    Environment* parent;
    bm_vector* variables;
    bm_vector* functions;
};

struct Function
{
    wchar* name;
    bm_vector* arguments;
    Environment* envi;
};

struct Data
{
    wchar* name;
    DATA_TYPES type;
    union
    {
        Integer* i_num;
        Str* str;
        List* list;
        Float* f_num;
    }data;
    
};

struct Integer
{
    long long int value;
};

struct Str
{
    wchar* str;
    size_t len;
};

struct Float
{
    long double value;
};

struct List
{
    bm_vector* vec;
};

//----interpretator----
Data* inter_expr(Environment*, Expretion*);

//data
Data* inter_num(Environment*, Expretion*);
Data* inter_str(Environment*, Expretion*);
Data* inter_list(Environment*, Expretion*);
Data* inter_bool(Environment*, Expretion*);

//others
Data* inter_seque(Environment*, Expretion*);
Data* inter_func(Environment*, Expretion*);
Data* inter_call(Environment*, Expretion*);
Data* inter_if(Environment*, Expretion*);
Data* inter_var(Environment*, Expretion*);
Data* inter_index(Environment*, Expretion*);
Data* inter_bin(Environment*, Expretion*);
Data* inter_return(Environment*, Expretion*);
Data* inter_for(Environment*, Expretion*);
Data* inter_while(Environment*, Expretion*);


#endif