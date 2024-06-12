#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include"includes.h"
#include"parcer.h"

typedef struct Environment Environment;
typedef struct Function Function;
typedef struct Data Data;
typedef struct bm_vector bm_vector;

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
    expretion_Vector body;
};

struct Data
{
    wchar* name;
    DATA_TYPES type;
};

//bm_vector
struct bm_vector
{
    void** array;
    size_t size;
    size_t len;
    size_t element_size;
};
bm_vector* create_bm_vec(size_t);
void delete_bm_vec(bm_vector*);
Errno bm_vector_push(bm_vector*, void* el);
void bm_vector_resize(bm_vector*);
#endif