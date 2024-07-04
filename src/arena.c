#include"arena.h"


Arena* arena_create()
{
    Arena* ar = malloc(sizeof(Arena));
    ar->index = 0;
    ar->size = ARENA_BLOCK_SIZE;
    ar->region = malloc(ARENA_BLOCK_SIZE);
    return ar;
}

void* arena_alloc(Arena* arena, size_t size)
{
    size+=sizeof(size_t);
    if(arena->size - arena->index < size)
    {
        arena->region = realloc(arena->region, arena->size + ARENA_BLOCK_SIZE);
        arena->size += ARENA_BLOCK_SIZE;
    }
    arena->index += size;
    *(arena->region + arena->index - size) = size -sizeof(size_t);
    return arena->region + arena->index - size + sizeof(size_t);
}

void* arena_realloc(Arena* arena, void* loc, size_t size)
{
    char* dest = arena_alloc(arena, size);
    char* ptr = loc;
    for(size_t i = 0; i < (size_t)*(dest - sizeof(size_t)); ++i)
    {
        *(dest + i) = *(ptr + i);
        
    }
    return dest;
}

void arena_destroy(Arena* arena)
{
    bm_free(arena->region);
    bm_free(arena);
}
