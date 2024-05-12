#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include<includes.h>

typedef enum token_types
{
    KEYWORD = 0,
    STRING,
    INTEGER,
    DOUBLE,
    START,
    END
}token_type_t;
typedef struct node
{
    struct node* next;
    struct node* prev;

    token_type_t tk_type;
    
    char* value;
}node_t;

node_t *new_node(token_type_t tk_type, char* value);
void push_back(node_t* destination, node_t* node);
void del_node(node_t* node);

#endif