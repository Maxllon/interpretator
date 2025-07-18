#include<globals.h>

struct Region* new_region(size_t size)
{
    struct Region* r = malloc(sizeof(struct Region));
    r->size = size;
    r->next = NULL;
    r->data = malloc(size);
    r->head = r->data;
    return r;
}

Arena* arena_create()
{
    Arena* ar = malloc(sizeof(Arena));
    ar->begin = new_region(REGION_DEFAULT_CAPACITY);
    ar->end = ar->begin;
    return ar;
}

void* arena_alloc(size_t size)
{
    Arena* arena = ARENA;
    size+=sizeof(size_t);
    if(arena->end->size < size)
    {
        if(arena->end == arena->begin)
        {
            arena->end = new_region(REGION_DEFAULT_CAPACITY+size);
            arena->begin->next = arena->end;
        }
        else
        {
            arena->end->next = new_region(REGION_DEFAULT_CAPACITY+size*10);
            arena->end = arena->end->next;
        }
    }
    *((size_t*)arena->end->head) = size-sizeof(size_t);
    char* res = arena->end->head + sizeof(size_t);
    arena->end->head += size;
    arena->end->size -= size;
    return res;
}

void* arena_realloc(void* loc, size_t size)
{
    char* dest = arena_alloc(size);
    char* ptr = loc;
    for(size_t i = 0; i < *((size_t*)((char*)loc - sizeof(size_t))); ++i)
    {
        dest[i] = ptr[i];
    }
    return dest;
}

void arena_destroy()
{
    Arena* arena = ARENA;
    Region* r1 = arena->begin;
    Region* r2;
    while(r1!=NULL)
    {
        r2 = r1->next;
        free(r1->data);
        free(r1);
        r1 = r2;
    }
    free(arena);
}