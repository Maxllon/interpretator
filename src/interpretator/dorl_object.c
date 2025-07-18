#include<dorl_object.h>
#include<long_num.h>

dorl_object* dorl_object_create(object_type type)
{
    dorl_object* res = arena_alloc(sizeof(dorl_object));
    if(!res) return NULL;
    res->type = type;
    res->methods = environment_create(NULL);
    if(res->methods == NULL) return NULL;
    if(type == OBJ_FUNC)
    {
        res->obj.func = arena_alloc(sizeof(func_obj));
    }  
    return res;
}

dorl_object* dorl_object_copy(dorl_object* obj)
{
    if(!obj) return NULL;
    dorl_object* res = dorl_object_create(obj->type);
    switch(obj->type)
    {
        case OBJ_NUM:
            res->obj.num = big_num_from_str(str_from_big_num(obj->obj.num));
            break;
        case OBJ_STR:
            res->obj.str = str_init(obj->obj.str->_str);
            break;
        case OBJ_FUNC:
            res = obj;
            break;
        case OBJ_LIST:
            size_t n = dk_get_size(obj->obj.root);
            dk_node* root = NULL;
            for(size_t i=0; i < n; ++i)
            {
                root = dk_merge(root, dk_new_node(dorl_object_copy(dk_get_el(obj->obj.root, i))));
                if(!root) return NULL;
            }
            res->obj.root = root;
            break;
        case OBJ_CLASS:
            res->methods = environment_copy(obj->methods);
            break;
        case OBJ_BOOL:
            res->obj._bool = obj->obj._bool;
            break;
        case OBJ_VOID:
            break;
        case OBJ_RETURN:
            res->obj.ret_value = dorl_object_copy(obj->obj.ret_value);
            break;
        case OBJ_INSTRYCTION:
            res->obj.instr_type = obj->obj.instr_type;
            break;
        default:
            printf("Неизестный тип объекта\n");
            arena_destroy();
            exit(1);
    }
    return res;
}
dorl_object* dorl_object_get(dorl_object* obj)
{
    if(obj->type == OBJ_LIST || obj->type == OBJ_STR) return obj;
    return dorl_object_copy(obj);
}

environment* environment_create(environment* parent)
{
    environment* res = arena_alloc(sizeof(environment));
    if(res == NULL) return NULL;
    res->parent = parent;
    res->var_list = NULL;
    return res;
}
dorl_object* environment_get(string* name, environment* dest)
{
    if(dest == NULL) return NULL;
    link_node* list = dest->var_list;
    while(list != NULL)
    {
        pair* p = (pair*)list->value;
        if(str_equal(name, (string*)p->first)) return (dorl_object*)p->second;
        list = list->next;
    }
    if(dest->parent)
    {
        return environment_get(name, dest->parent);
    }
    return NULL;
}
environment* environment_copy(environment* envi)
{
    if(!envi) return NULL;
    environment* parent = envi->parent;
    environment* res = environment_create(parent);
    link_node* list = res->var_list;
    link_node* src = envi->var_list;
    while(src != NULL)
    {
        pair* p = arena_alloc(sizeof(pair));
        pair* src_p = (pair*) src->value;
        p->first = str_init(((string*)src_p->first)->_str);
        p->second = dorl_object_copy((dorl_object*)src_p->second);
        list = link_push_back(list, link_new(p));
        src = src->next;
    }
    res->var_list = link_go_start(list);
    return res;
}
int environment_change(string* name, dorl_object* obj, environment* dest)
{
    if(!dest) return -1;
    link_node* list = dest->var_list;
    while(list)
    {
        pair* p = (pair*)list->value;
        if(str_equal(name, (string*)p->first))
        {
            p->second = obj;
            return ALL_GOOD;
        }
        list = list->next;
    }
    pair* p = arena_alloc(sizeof(pair));
    p->first = str_init(name->_str);
    p->second = obj;
    dest->var_list = link_push_back(dest->var_list, link_new(p));
    dest->var_list = link_go_start(dest->var_list);
    return ALL_GOOD;
}