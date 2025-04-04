#include"bmpl_object.h"

bmpl_object* new_object(bmpl_object_types type, void* value, Arena* arena)
{
    bmpl_object* obj = arena_alloc(arena, sizeof(bmpl_object));
    obj->type = type;
    switch (type)
    {
        case VOID_OBJ:
            break;
        case NUM_OBJ:
            obj->num = value;
            break;
        case STR_OBJ:
            obj->str = value;
            break;
        case BOOL_OBJ:
            obj->_bool = *(int*)value;
            break;
        case FUNC_OBJ:
            obj->func = value;
            break;
        case LIST_OBJ:
            obj->root = value;
            break;
        default:
            wprintf(L"Интерпретатор: неизвестный тип объекта\n");
            return NULL;
    }
    return obj;
}

static dk_node* dk_copy(dk_node* root, Arena* arena)
{
    if(!root) return NULL;
    dk_node* res = dk_new_node(copy_object(root->value, arena), arena);
    res->prior = root->prior;
    res->sons = root->sons;
    res->left = dk_copy(root->left, arena);
    res->right = dk_copy(root->right, arena);
    return res;
}
bmpl_object* copy_object(bmpl_object* src, Arena* arena)
{
    bmpl_object* obj = new_object(src->type, NULL, arena);
    switch (obj->type)
    {
        case VOID_OBJ:
            break;
        case NUM_OBJ:
            obj->num = big_num_from_str(str_from_big_num(src->num, arena), arena);
            break;
        case STR_OBJ:
            obj->str = bmpl_string_copy(src->str, arena);
            break;
        case BOOL_OBJ:
            obj->_bool = src->_bool;
            break;
        case FUNC_OBJ:
            obj->func = src->func;
            break;
        case LIST_OBJ:
            obj->root = dk_copy(src->root, arena);
            break;
        default:
            wprintf(L"Интерпретатор: неизвестный тип объекта\n");
            return NULL;
    }
    return obj;
}

module* new_module(Arena* arena, module* parent)
{
    module* block = arena_alloc(arena, sizeof(module));
    block->parent = parent;
    block->variables = NULL;
    return block;
}

variable* new_variable(bmpl_string* name, bmpl_object* value, Arena* arena)
{
    variable* var = arena_alloc(arena, sizeof(variable));
    var->name = bmpl_string_copy(name, arena);
    var->value = value;
    return var;
}

bmpl_object* find_var(module* main, bmpl_string* str)
{
    if(!main) return NULL;
    for(size_t i = 0; i<main->variables->sons; ++i)
    {
        variable* var = dk_get_el(main->variables, i);
        if(bmpl_string_equal(var, str)) return var->value;
    }
    if(main->parent) return find_var(main->parent, str);
    return NULL;
}

void add_var(module* main, variable* var, Arena* arena)
{
    dk_node* node = dk_new_node(var, arena);
    main->variables = dk_merge(main->variables, node);
}