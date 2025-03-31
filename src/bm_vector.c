#include <stdlib.h> // Для NULL
#include"bm_vector.h"


bm_vector* bm_vector_create(Arena* arena)
{
    bm_vector* vec = arena_alloc(arena, sizeof(bm_vector));
    if (!vec) return NULL; // Проверка на выделение памяти

    vec->data = arena_alloc(arena, sizeof(void*) * BM_VECTOR_BLOCK);

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
    if (vec->len >= vec->capacity)
    {
        void** new_data = arena_realloc(vec->arena, vec->data, sizeof(void*) * (vec->capacity + BM_VECTOR_BLOCK));
        if (!new_data) 
        {
            fprintf(stderr, "Ошибка в vector_push\n");
            return;
        }

        vec->data = new_data;
        for(size_t i = vec->capacity; i < vec->capacity + BM_VECTOR_BLOCK; ++i)
        {
            vec->data[i] = NULL;
        }
        vec->capacity += BM_VECTOR_BLOCK;
    }

    vec->data[vec->len] = item;
    vec->len++;
}


void* bm_vector_at(bm_vector* vec, size_t index)
{
    if(index >= vec->len)
    {
        return NULL;
    }
    return vec->data[index];
}


void bm_vector_change(bm_vector* vec, size_t index, void* item)
{
    if (index >= vec->capacity)
    {
        size_t new_capacity = vec->capacity;
        while (index >= new_capacity) {
            new_capacity += BM_VECTOR_BLOCK;
        }

        void** new_data = arena_realloc(vec->arena, vec->data, sizeof(void*) * new_capacity);

        if (!new_data) {
            fprintf(stderr, "Ошибка в vector_change\n");
            return;
        }

        vec->data = new_data;
        for (size_t i = vec->capacity; i < new_capacity; ++i) {
            vec->data[i] = NULL;
        }
        vec->capacity = new_capacity;
    }

    if (index >= vec->len) {
        vec->len = index + 1;
    }
    vec->data[index] = item;
}