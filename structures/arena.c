#include<arena.h>

Arena* arena_init()
{
    Arena* arena = malloc(sizeof(Arena));
    arena->memory = malloc(ARENA_MEM_BLOCK);
    arena->size = ARENA_MEM_BLOCK;
    arena->used = 0;
    return arena;
}

void* arena_alloc(Arena* arena, size_t size)
{
    if(arena->size - arena->used < size)
    {
        size_t new_size = arena->size;
        while(new_size - arena->used < size) new_size<<1;
        arena->memory = realloc(arena->memory, new_size);
    }
    arena->used += size;
    return (arena->memory + arena->used - size);
}
void arena_destroy(Arena* arena)
{
    free(arena->memory);
    free(arena);
}