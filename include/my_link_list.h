#ifndef LINK_LIST_H
#define LINK_LIST_H

typedef struct link_node link_node;

struct link_node
{   
    link_node* next;
    link_node* pref;

    void* value;
};

link_node*
link_new(void* data);

link_node*
link_go_start(link_node* root);

link_node*
link_go_end(link_node* root);

link_node*
link_push_back(link_node* root, link_node* node);

link_node*
link_push_front(link_node* root, link_node* node);

#endif