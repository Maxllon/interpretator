#ifndef ARENA_H
#define ARENA_H

#include"includes.h"

typedef struct Arena
{
    char* region;
    size_t index;
    size_t size;
}Arena;

Arena* arena_create(size_t);
void* arena_alloc(Arena*, size_t);
void arena_destroy(Arena*);

#endif