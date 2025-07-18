#ifndef MY_LIST_H
#define MY_LIST_H

#include<defines.h>

typedef struct vector vector;
struct vector
{
    void** buff;
    size_t capacity;
    size_t len;
};

vector*
list_init();

int
list_push(vector* list, void* value);

void*
list_at(vector* list, size_t index);
int
list_change(vector* list, void* value, size_t index);

int
list_clear(vector* list);

#endif
