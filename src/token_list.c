#include<token_list.h>
node_t *new_node(token_type_t tk_type, char* value)
{
    node_t *node = malloc(sizeof(node_t));
    node->next = NULL;
    node->prev = NULL;
    node->tk_type = tk_type;
    node->value = malloc(strlen(value) + 1);
    node->value = strcpy(node->value, value);
    return node;
}

void push_back(node_t* destination, node_t* node)
{
    if(destination == NULL)
    {
        printf("Error: cant push back. The destination is NULL\n");
        return;
    }
    while(destination->next != NULL)
    {
        destination = destination->next;
    }
    destination->next = node;
}
void del_node(node_t* node)
{
    if(node->next != NULL) del_node(node->next);
    free(node->value);
    free(node);
}