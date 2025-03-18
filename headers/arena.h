#ifndef ARENA_H
#define ARENA_H

#include<defines.h>

typedef struct
{
    byte* memory;
    size_t used;
    size_t size;
}Arena;

Arena* arena_init();
void* arena_alloc(Arena*, size_t);
void arena_destroy(Arena*);

#endif