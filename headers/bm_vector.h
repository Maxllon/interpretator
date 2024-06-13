#ifndef BM_VECTOR_H
#define BM_VECTOR_H

#include"includes.h"

#define BM_VECTOR_BLOCK 100

typedef struct bm_vector bm_vector;
struct bm_vector
{
    void** data;
    size_t len;
    size_t capacity;
};

bm_vector* bm_vector_create(void);
Errno bm_vector_free(bm_vector*);
void bm_vector_push(bm_vector*, void*);
void* bm_vector_at(bm_vector*, size_t);

#endif