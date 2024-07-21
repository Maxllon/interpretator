#include"bm_vector.h"

bm_vector* bm_vector_create(Arena* arena)
{
    bm_vector* vec = arena_alloc(arena, sizeof(bm_vector));
    vec->data = arena_alloc(arena, BM_VECTOR_BLOCK);
    for(size_t i = 0; i < BM_VECTOR_BLOCK; ++i)
    {
        vec->data[i] = NULL;
    }
    vec->capacity = BM_VECTOR_BLOCK;
    vec->len = 0;
    vec->arena = arena;
    return vec;
}

void bm_vector_push(bm_vector* vec, void* item)
{
    vec->len++;
    if(vec->len > vec->capacity)
    {
        vec->data = arena_realloc(vec->arena, vec->data, sizeof(void*) * (vec->capacity + BM_VECTOR_BLOCK));
        for(size_t i = vec->capacity; i < vec->capacity + BM_VECTOR_BLOCK; ++i)
        {
            vec->data[i] = NULL;
        }
        vec->capacity += BM_VECTOR_BLOCK;
    }
    vec->data[vec->len-1] = item;
}

void* bm_vector_at(bm_vector* vec, size_t index)
{
    if(vec->len <= index) return NULL;
    return vec->data[index];
}
void bm_vector_change(bm_vector* vec, size_t index, void* item)
{
    if(index >= vec->len) vec->len = index + 1;
    if(vec->len > vec->capacity)
    {
        vec->data = arena_realloc(vec->arena, vec->data, sizeof(void*) * (vec->capacity + BM_VECTOR_BLOCK));
        for(size_t i = vec->capacity; i < vec->capacity + BM_VECTOR_BLOCK; ++i)
        {
            vec->data[i] = NULL;
        }
        vec->capacity += BM_VECTOR_BLOCK;
    }
    vec->data[index] = item;
}

