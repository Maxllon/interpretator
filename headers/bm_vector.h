#ifndef BM_VECTOR_H
#define BM_VECTOR_H

#include"includes.h"
#include"arena.h"

#define BM_VECTOR_BLOCK 128

typedef struct bm_vector bm_vector;
struct bm_vector
{
    void** data;
    size_t len;
    size_t capacity;
    Arena* arena;
};

bm_vector* bm_vector_create(Arena*);
void bm_vector_push(bm_vector*, void*);
void* bm_vector_at(bm_vector*, size_t);
void bm_vector_change(bm_vector*, size_t, void*);

#endif