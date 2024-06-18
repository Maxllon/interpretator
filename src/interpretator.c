#include"interpretator.h"
#include"arena.h"

static Arena* ARENA = NULL;

Environment* create_empty_environment(Environment* parent)
{
    Environment* envi = arena_alloc(ARENA, sizeof(envi));
    envi->parent = parent;
    envi->variables = bm_vector_create();
    return envi;
}