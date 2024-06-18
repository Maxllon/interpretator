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
    if(arena->size - arena->index < size)
    {
        arena->region = realloc(arena->region, arena->size + ARENA_BLOCK_SIZE);
        arena->size += ARENA_BLOCK_SIZE;
    }
    arena->index += size;
    return arena->region + arena->index - size;
}

void arena_destroy(Arena* arena)
{
    bm_free(arena->region);
    bm_free(arena);
}
