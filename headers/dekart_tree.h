#ifndef DEKART_TREE_H
#define DEKART_TREE_H

#include"includes.h"
#include"arena.h"
#include"bm_vector.h"

typedef struct dk_node dk_node;

struct dk_node
{
    size_t prior;
    size_t sons;

    void* value;

    dk_node* left;
    dk_node* right;
};

dk_node* dk_new_node(void* value, Arena* arena);
size_t dk_get_size(dk_node* root);

pair dk_split(dk_node* root, size_t key);
dk_node* dk_merge(dk_node* L, dk_node* R);

void dk_add(void* value, size_t index);
void* dk_get_el(dk_node* root, size_t key);
void dk_erase(dk_node* root, size_t key);

bm_vector* dk_get_static(dk_node* root);

void dk_output(dk_node* root);

#endif