#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include<includes.h>

enum token_types
{
    KEYWORD,
    STRING,
    INTEGER,
    DOUBLE
};
typedef union value
{
    long long int i;
    long double f;
    char* s;
}value_t;
typedef struct node
{
    node_t* next;
    node_t* prev;

    char* tk_type;
    
    value_t value;
}node_t;

node_t* new_node(char* tk_type, value_t value);
void del_node(node_t* node);
typedef struct tk_list
{
    node_t* father;

    node_t* get();
    node_t* previous();
    node_t* next();
}tk_list_t;

#endif