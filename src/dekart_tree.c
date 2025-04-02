#include"dekart_tree.h"

dk_node* dk_new_node(void* value, Arena* arena)
{
    dk_node* root = arena_alloc(arena, sizeof(dk_node));
    root->left = NULL;
    root->right = NULL;
    root->sons = 1;
    root->prior = rand();
    root->value = value;
    return root;
}
size_t dk_get_size(dk_node* root)
{
    return root ? root->sons : 0;
}
static void upd(dk_node* root)
{
    if(root)
    {
        root->sons = dk_get_size(root->left) + dk_get_size(root->right) + 1;
    }
}
pair dk_split(dk_node* root, size_t key)
{
    pair res;
    if(!root)
    {
        res.first = NULL;
        res.second = NULL;
        return res;
    }

    size_t current_key = dk_get_size(root->left) + 1;
    if(current_key <= key)
    {
        res = dk_split(root->right, key - current_key);
        root->right = res.first;
        res.first = root;
    }
    else
    {
        res = dk_split(root->left, key);
        root->left = res.second;
        res.second = root;
    }
    upd(root);
    return res;

}

dk_node* dk_merge(dk_node* L, dk_node* R)
{
    if(!L || !R) return L ? L : R;

    dk_node* res;
    if(L->prior > R->prior)
    {
        L->right = dk_merge(L->right, R);
        res = L;
    }
    else
    {
        R->left = dk_merge(L, R->left);
        res = R;
    }
    upd(res);
    return res;
}

void dk_output(dk_node* root)
{
    if(!root) return;
    dk_output(root->left);
    wprintf(L"%llu ", *(size_t*)root->value);
    dk_output(root->right);
}