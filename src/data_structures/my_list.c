#include<my_list.h>
#include<globals.h>

vector*
list_init()
{
    vector* res = arena_alloc(sizeof(vector));
    if(res == NULL)
    {
        LAST_ERROR.type = BAD_ALLOC;
        return NULL;
    }
    res->buff = arena_alloc(sizeof(void*) * LIST_DEFAULTT_LEN);
    if(res->buff == NULL)
    {
        LAST_ERROR.type = BAD_ALLOC;
        return NULL;
    }
    res->capacity = LIST_DEFAULTT_LEN;
    res->len = 0;
    return res;
}

int
list_push(vector* list, void* value)
{
    list->len++;
    if(list->len > list->capacity)
    {
        list->buff = arena_realloc(list->buff, sizeof(void*) * list->len * 2);
        list->capacity = list->len * 2;
        if(!list->buff)
        {
            LAST_ERROR.type = BAD_ALLOC;
            return -1; 
        }
        memset(list->buff + list->len, 0, list->len);
    }
    list->buff[list->len-1] = value;
    return 0;
}

void*
list_at(vector* list, size_t index)
{
    if(index >= list->capacity)
    {
        list->len = index;
        list->buff = arena_realloc(list->buff, sizeof(void*) * index * 2);
        list->capacity = index * 2;
        if(!list->buff)
        {
            LAST_ERROR.type = BAD_ALLOC;
            return NULL; 
        }
        memset(list->buff + list->len, 0, list->len);
    }   
    return list->buff[index];
}

int
list_change(vector* list, void* value, size_t index)
{
    if(index >= list->capacity)
    {
        list->len = index;
        list->buff = arena_realloc(list->buff, sizeof(void*) * index * 2);
        list->capacity = index * 2;
        if(!list->buff)
        {
            LAST_ERROR.type = BAD_ALLOC;
            return -1; 
        }
        memset(list->buff + list->len, 0, list->len);
    } 
    if (index >= list->len) 
    {
        list->len = index;
    }  
    list->buff[index] = value;
    return 0;
}

int
list_clear(vector* list)
{
    free(list->buff);
    free(list);
    return 0;
}
