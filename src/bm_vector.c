#include"bm_vector.h"

bm_vector* bm_vector_create(Arena* arena)
{
    bm_vector* vec = arena_alloc(arena, sizeof(bm_vector));
    vec->data = arena_alloc(arena, BM_VECTOR_BLOCK);
    vec->capacity = BM_VECTOR_BLOCK;
    vec->len = 0;
    return vec;
}

void bm_vector_push(bm_vector* vec, void* item, Arena* arena)
{
    vec->len++;
    if(vec->len > vec->capacity)
    {
        vec->data = arena_realloc(arena, vec->data, sizeof(void*) * (vec->capacity + BM_VECTOR_BLOCK));
        vec->capacity += BM_VECTOR_BLOCK;
    }
    vec->data[vec->len-1] = item;
}

void* bm_vector_at(bm_vector* vec, size_t index)
{
    return vec->data[index];
}

