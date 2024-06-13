#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include"includes.h"
#include"parcer.h"
#include"bm_vector.h"

typedef struct Environment Environment;
typedef struct Function Function;
typedef struct Data Data;
typedef struct bm_vector bm_vector;
typedef struct Exprtion Expretion;

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
    bm_vector variables;
    bm_vector functions;
};

struct Function
{
    wchar* name;
    expretion_Vector arguments;
    Environment* envi;
};

struct Data
{
    wchar* name;
    DATA_TYPES type;
};

//interpretator
Data* inter_expr(Environment*, Expretion*);


//data
Data* inter_num(Environment*, Expretion*);
Data* inter_str(Environment*, Expretion*);
Data* inter_list(Environment*, Expretion*);
Data* inter_bool(Environment*, Expretion*);


#endif