#include<arena.h>

Arena* arena_init()
{
    Arena* arena = malloc(sizeof(Arena));
    if(arena == NULL)
    {
        perror("arena_init: malloc failed");
        return NULL;
    }
    arena->memory = malloc(ARENA_MEM_BLOCK);
    if(arena->memory == NULL)
    {
        perror("arena_init: malloc for memory failed");
        free(arena);
        return NULL;
    }
    arena->size = ARENA_MEM_BLOCK;
    arena->used = 0;
    return arena;
}

void* arena_alloc(Arena* arena, size_t size)
{
    if(arena == NULL) return NULL;
    size+=sizeof(size_t);
    if(arena->size - arena->used < size)
    {
        size_t new_size = arena->size;
        while(new_size - arena->used < size)
        {
            if(size > (SIZE_MAX >> 1))
            {
                fprintf(stderr, "arena alloc failed: out of memory. Request to large.\n");
                return NULL;
            }
            new_size<<=1;
        }
        arena->memory = realloc(arena->memory, new_size);
        if(arena->memory == NULL)
        {
            perror("arena alloc: realloc failed");
            return NULL;
        }
        arena->size = new_size;
    }
    *(size_t*) (arena->memory + arena->used) = size-sizeof(size_t);
    arena->used += size;
    return (arena->memory + arena->used - size + sizeof(size_t));
}
void* arena_realloc(Arena* arena, void* block, size_t size)
{
    if(arena == NULL) return NULL;
    if(block == NULL) return arena_alloc(arena, size);
    size_t old_size = get_block_size(block);
    void* new_block = arena_alloc(arena, size);
    if(new_block == NULL)
    {
        return NULL;
    }
    memcpy(new_block, block, old_size);
    return new_block;
}

void arena_destroy(Arena* arena)
{
    free(arena->memory);
    free(arena);
}