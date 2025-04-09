#include"dekart_tree.h"

dk_node* dk_new_node(void* value, Arena* arena)
{
    dk_node* root = arena_alloc(arena, sizeof(dk_node));
    root->left = NULL;
    root->right = NULL;
    root->sons = big_num_from_str(L"1", arena);
    root->prior = rand();
    root->value = value;
    return root;
}
big_num* dk_get_size(dk_node* root, Arena* arena)
{
    return root ? root->sons : big_num_from_str(L"0", arena);
}
static void upd(dk_node* root, Arena* arena)
{
    if(root)
    {
        root->sons = sum_big(sum_big(dk_get_size(root->left, arena), dk_get_size(root->right, arena), arena), big_num_from_str(L"1", arena), arena);
    }
}
pair dk_split(dk_node* root, big_num* key, Arena* arena)
{
    pair res;
    if(!root)
    {
        res.first = NULL;
        res.second = NULL;
        return res;
    }

    big_num* current_key = sum_big(dk_get_size(root->left, arena), big_num_from_str(L"1", arena), arena);
    int cmp = compare_big(current_key, key);
    if(cmp<=0)
    {
        res = dk_split(root->right, sub_big(key, current_key, arena), arena);
        root->right = res.first;
        res.first = root;
    }
    else
    {
        res = dk_split(root->left, key, arena);
        root->left = res.second;
        res.second = root;
    }
    upd(root, arena);
    return res;

}

dk_node* dk_merge(dk_node* L, dk_node* R, Arena* arena)
{
    if(!L || !R) return L ? L : R;

    dk_node* res;
    if(L->prior > R->prior)
    {
        L->right = dk_merge(L->right, R, arena);
        res = L;
    }
    else
    {
        R->left = dk_merge(L, R->left, arena);
        res = R;
    }
    upd(res, arena);
    return res;
}

dk_node* dk_add(dk_node* root, dk_node* T , big_num* pos, Arena* arena)
{
    pair p = dk_split(root, pos, arena);
    dk_node* res = dk_merge(p.first, T, arena);
    res = dk_merge(res, p.second, arena);
    return res;
}
void* dk_get_el(dk_node* root, big_num* pos, Arena* arena)
{
    pair p = dk_split(root, pos, arena);
    p = dk_split(p.second, big_num_from_str(L"1", arena), arena);
    return ((dk_node*)p.first)->value;
}
dk_node* dk_erase(dk_node* root, big_num* pos, Arena* arena)
{
    pair p = dk_split(root, pos, arena);
    pair p2 = dk_split(p.second, big_num_from_str(L"1", arena), arena);
    return dk_merge(p.first, p2.second, arena);
}

void dk_output(dk_node* root)
{
    if(!root) return;
    dk_output(root->left);
    wprintf(L"%llu ", *(size_t*)root->value);
    dk_output(root->right);
}