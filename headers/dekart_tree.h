#ifndef DEKART_TREE_H
#define DEKART_TREE_H

#include"includes.h"
#include"arena.h"
#include"bm_vector.h"
#include"long_num.h"

typedef struct dk_node dk_node;

struct dk_node
{
    size_t prior;
    big_num* sons;

    void* value;

    dk_node* left;
    dk_node* right;
};

dk_node* dk_new_node(void* value, Arena* arena);
big_num* dk_get_size(dk_node* root, Arena* arena);

pair dk_split(dk_node* root, big_num* key, Arena* arena);
dk_node* dk_merge(dk_node* L, dk_node* R, Arena* arena);

dk_node* dk_add(dk_node* root, dk_node* T , big_num* pos, Arena* arena);
void* dk_get_el(dk_node* root, big_num* pos, Arena* arena);
dk_node* dk_erase(dk_node* root, big_num* key, Arena* arena);

void dk_output(dk_node* root);

#endif