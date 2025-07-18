#include<my_link_list.h>
#include<globals.h>

link_node*
link_new(void* data)
{
    link_node* root = arena_alloc(sizeof(link_node));
    if(root == NULL) return NULL;
    root->next = NULL;
    root->pref = NULL;
    root->value = data;
    return root;
}

link_node*
link_go_start(link_node* root)
{
    if(root == NULL) return NULL;
    while(root->pref != NULL) root = root->pref;
    return root;
}

link_node*
link_go_end(link_node* root)
{
    if(root == NULL) return NULL;
    while(root->next != NULL) root = root->next;
    return root;
}

link_node*
link_push_back(link_node* root, link_node* node)
{
    if(root == NULL) return node;
    if(node == NULL) return NULL;
    root = link_go_end(root);
    root->next = node;
    node->pref = root;
    return node;
}

link_node*
link_push_front(link_node* root, link_node* node)
{
    if(root == NULL) return node;
    root = link_go_start(root);
    root->pref = node;
    node->next = root;
    return node;
}
    