#ifndef ARENA_H
#define ARENA_H

#include"includes.h"

typedef struct Arena Arena;
typedef struct Region Region;

struct Region
{
    size_t size;
    char* data;
    char* head;
    size_t used;
    Region* next;
};
struct Arena
{
    Region* begin;
    Region* end;
};

Region* new_region(size_t size);
Arena* arena_create();
void* arena_alloc(Arena* arena, size_t size);
void* arena_realloc(Arena* arena, void* loc, size_t size);
void arena_destroy(Arena* arena);


#endif