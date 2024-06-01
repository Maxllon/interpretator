#ifndef MEM_CONTROL_H
#define MEM_CONTROL_H

#include "includes.h"

typedef struct node_t
{
    struct node_t* next;
    void* ptr;
}node;
void mem_add(void* ptr);
void free_all(node* main);

#endif