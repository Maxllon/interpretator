#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include<includes.h>

typedef enum token_types
{
    KEYWORD = 0,
    STRING,
    INTEGER,
    DOUBLE
}token_type_t;
typedef struct node
{
    node_t* next;
    node_t* prev;

    token_type_t tk_type;
    
    char* value;
}node_t;

node_t* new_node(token_type_t tk_type, char* value);
void del_node(node_t* node);
typedef struct tk_list
{
    node_t* father;

    node_t* get();
    node_t* previous();
    node_t* next();
}tk_list_t;

#endif