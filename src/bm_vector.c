#include"bm_vector.h"

bm_vector* bm_vector_create(void)
{
    bm_vector* vec = malloc(sizeof(bm_vector));
    vec->data = calloc(BM_VECTOR_BLOCK, sizeof(void*));
    vec->capacity = BM_VECTOR_BLOCK;
    vec->len = 0;
    return vec;
}

Errno bm_vector_free(bm_vector* vec)
{
    if(vec == NULL) return 1;
    free(vec->data);
    free(vec);
    return 0;
}

void bm_vector_push(bm_vector* vec, void* item)
{
    vec->len++;
    if(vec->len > vec->capacity)
    {
        vec->data = realloc(vec->data, sizeof(void*) * (vec->capacity + BM_VECTOR_BLOCK));
        vec->capacity += BM_VECTOR_BLOCK;
    }
    vec->data[vec->len-1] = item;
}

void* bm_vector_at(bm_vector* vec, size_t index)
{
    return vec->data[index];
}

