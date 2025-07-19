#include<interpretator.h>
#include<globals.h>

environment* str_methods = NULL;
environment* list_methods = NULL;



static dorl_object* 
interpretate_atom(expression* expr);
static dorl_object*
interpretate_seque(expression* expr);
static dorl_object*
interpretate_var(expression* expr);
static dorl_object*
bin_assign(expression* left, expression* right);

int ise = 0;

static size_t
get_real_index(dorl_object* index, dorl_object* dest)
{
    if(index->type != OBJ_NUM)
    {
        printf("Неправильный индекс!\n");
        arena_destroy();
        exit(1);
    }
    if(dest->type != OBJ_LIST && dest->type != OBJ_STR)
    {
        printf("Неправильный индекс!\n");
        arena_destroy();
        exit(1);
    }
    size_t pos = szt_from_big_num(index->obj.num);
    size_t sz;
    if(dest->type == OBJ_LIST)
    {
        sz = dk_get_size(dest->obj.root);
    }
    else
    {
        sz = dest->obj.str->len;
    }
    if(ise) sz++;
    if(index->obj.num->is_negative)
    {
        if(pos > sz)
        {
            printf("Неправильный индекс!\n");
            arena_destroy();
            exit(1);
        }
        pos = sz - pos;
    }
    if(pos >= sz)
    {
        printf("Неправильный индекс!\n");
        arena_destroy();
        exit(1);
    } 
    return pos;
}

static dorl_object* void_obj;
environment* CURRENT_ENVI;

void 
func_create(char32_t* name, size_t arg_cnt, void* func)
{
    dorl_object* res = dorl_object_create(OBJ_FUNC);
    res->obj.func->is_dorl = False;
    res->obj.func->cnt = arg_cnt;
    res->obj.func->src.c_func = func;
    environment_change(str_init(name), res, CURRENT_ENVI);
}

static void 
out_once(dorl_object* obj)
{
    switch(obj->type)
    {
        case OBJ_STR:
            str_out(obj->obj.str);
            break;
        case OBJ_NUM:
            str_out(str_init(str_from_big_num(obj->obj.num)));
            break;
        case OBJ_BOOL:
            if(obj->obj._bool) str_out(str_init(U"Истина"));
            else str_out(str_init(U"Ложь"));
            break;
        case OBJ_VOID:
            str_out(str_init(U"<ПУСТОТА>"));
            break;
        case OBJ_CLASS:
            str_out(str_init(U"<ОБЪЕКТ>"));
            break;
        case OBJ_FUNC:
            str_out(str_init(U"<ФУНКЦИЯ>"));
            break;    
        case OBJ_INSTRYCTION:
            str_out(str_init(U"<ИНСТРУКЦИЯ>"));
            break;
        case OBJ_RETURN:
            str_out(str_init(U"<ВОЗВРАЩЕНИЕ>"));
            break;
        case OBJ_LIST:
            putchar(U'[');
            dk_node* root = obj->obj.root;
            for(size_t i=0;i<dk_get_size(root); ++i)
            {
                out_once(dk_get_el(root, i));
                if(i != dk_get_size(root) - 1)
                {
                    putchar(U',');
                    putchar(U' ');
                }
            }
            putchar(U']');
            break;
        default:
            printf("Ошибка вывода: Неизвестный тип для вывода: %d\n", obj->type);
            arena_destroy();
            exit(1);
    }
}   

static dorl_object*
STD_OUT(link_node* val)
{
    dorl_object* value = val->value;
    out_once(value);
    return void_obj;
}

static dorl_object*
STD_STR_REV(link_node* val)
{
    dorl_object* str = environment_get(str_init(U"self"), CURRENT_ENVI);
    str_reverse(str->obj.str);
    return str;
}

static dorl_object*
STD_STR_CUT(link_node* val)
{
    dorl_object* str = environment_get(str_init(U"self"), CURRENT_ENVI);
    dorl_object* start = val->value;
    val = val->next;
    dorl_object* end = val->value;
    if(start->type != OBJ_NUM || end->type != OBJ_NUM)
    {
        printf("Срез ожидает два числа\n");
        arena_destroy();
        exit(1);
    }
    size_t pos_start = get_real_index(start, str);
    size_t pos_end = get_real_index(end, str);
    if(start > end)
    {
        printf("Некорректный срез!\n");
        arena_destroy();
        exit(1);
    }
    str_cut(str->obj.str, pos_start, pos_end);
    return str;
}

static dorl_object*
STD_LIST_ADD(link_node* val)
{
    dorl_object* list = environment_get(str_init(U"self"), CURRENT_ENVI);
    dorl_object* el = val->value;
    val = val->next;
    dorl_object* index = val->value;
    ise = 1;
    size_t pos = get_real_index(index, list);
    ise = 0;
    list->obj.root = dk_add(list->obj.root, dk_new_node(dorl_object_copy(el)), pos);
    return void_obj;
}

static void
set_global()
{
    str_methods = environment_create(NULL);
    list_methods = environment_create(NULL);

    //str
    environment* global = CURRENT_ENVI;
    CURRENT_ENVI = str_methods;
    func_create(U"перевернуть", 0, STD_STR_REV);
    func_create(U"срез", 2, STD_STR_CUT);
    CURRENT_ENVI = global;

    //list
    global = CURRENT_ENVI;
    CURRENT_ENVI = list_methods;
    func_create(U"добавить", 2, STD_LIST_ADD);
    CURRENT_ENVI = global;

    func_create(U"печать", 1, STD_OUT);
}

static dorl_object*
interpretate_seque(expression* expr)
{
    link_node* seque = expr->expr.seque->list;
    if(!seque) return void_obj;
    dorl_object* res;
    while(seque)
    {
        res = interpretate_atom((expression*)seque->value);
        if(!res) return NULL;
        if(res->type == OBJ_INSTRYCTION) return res;
        if(res->type == OBJ_RETURN) return res;
        seque = seque->next;
    }
    return res;
}

static dorl_object*
interpretate_var(expression* expr)
{
    string* name = expr->expr.var->value;
    dorl_object* res = environment_get(name, CURRENT_ENVI);
    if(!res)
    {
        if(environment_change(name, void_obj, CURRENT_ENVI) != ALL_GOOD) return NULL;
        res = void_obj;
    }
    return res;
}

static dorl_object*
bin_assign(expression* left, expression* right)
{
    if(left->type != EXPR_VAR && left->type != EXPR_INDEX)
    {
        LAST_ERROR.pos_start = left->_pos;
        LAST_ERROR.type = INCORRECT_LEFT;
        return NULL;
    }
    dorl_object* r_val = dorl_object_copy(interpretate_atom(right));
    if(!r_val) return NULL;
    if(left->type == EXPR_VAR)
    {
        string* name = left->expr.var->value;
        if(environment_change(name, r_val, CURRENT_ENVI) != ALL_GOOD) return NULL;
        return r_val;
    }
    dorl_object* dest = interpretate_atom(left->expr.index->dest);
    if(!dest) return NULL;
    dorl_object* index = interpretate_atom(left->expr.index->index);
    if(!index) return NULL;
    if(index->type != OBJ_NUM)
    {
        LAST_ERROR.pos_start = left->expr.index->index->_pos;
        LAST_ERROR.type = INCORRECT_INDEX;
        return NULL;
    }
    if(dest->type != OBJ_LIST)
    {
        LAST_ERROR.pos_start = left->expr.index->index->_pos;
        LAST_ERROR.type = INCORRECT_ITER;
        return NULL;
    }
    size_t pos = szt_from_big_num(index->obj.num);
    size_t sz;
    sz = dk_get_size(dest->obj.root);
    if(index->obj.num->is_negative)
    {
        if(pos > sz)
        {
            LAST_ERROR.pos_start = left->expr.index->index->_pos;
            LAST_ERROR.type = INCORRECT_INDEX;
            return NULL;
        }
        pos = sz - pos;
    }
    if(pos >= sz)
    {
        LAST_ERROR.pos_start = left->expr.index->index->_pos;
        LAST_ERROR.type = INCORRECT_ITER;
        return NULL;
    }

    dorl_object* el = (dorl_object*)dk_get_el(dest->obj.root, pos);
    *el = *r_val;

    return r_val;
}

static dorl_object*
bin_num_num(dorl_object* left, dorl_object* right, op_type op)
{
    if(op == KEY_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(!compare_big_normal(left->obj.num, right->obj.num)) res->obj._bool = True;
        else res->obj._bool = False;
        return res;
    }
    if(op == KEY_NOT_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(!compare_big_normal(left->obj.num, right->obj.num)) res->obj._bool = False;
        else res->obj._bool = True;
        return res;  
    }
    if(op == KEY_GREATER)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(compare_big_normal(left->obj.num, right->obj.num) == 1) res->obj._bool = True;
        else res->obj._bool = False;
        return res;   
    }
    if(op == KEY_LESS)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(compare_big_normal(left->obj.num, right->obj.num) == -1) res->obj._bool = True;
        else res->obj._bool = False;
        return res;   
    }
    if(op == KEY_GREATER_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(compare_big_normal(left->obj.num, right->obj.num) != -1) res->obj._bool = True;
        else res->obj._bool = False;
        return res;   
    }
    if(op == KEY_LESS_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(compare_big_normal(left->obj.num, right->obj.num) != 1) res->obj._bool = True;
        else res->obj._bool = False;
        return res;   
    }
    if(op == KEY_PLUS)
    {
        dorl_object* res = dorl_object_create(OBJ_NUM);
        res->obj.num = sum_big(left->obj.num, right->obj.num);
        return res;
    }
    if(op == KEY_MINUS)
    {
        dorl_object* res = dorl_object_create(OBJ_NUM);
        res->obj.num = sub_big(left->obj.num, right->obj.num);
        return res;
    }
    if(op == KEY_MULT)
    {
        dorl_object* res = dorl_object_create(OBJ_NUM);
        res->obj.num = mult_big(left->obj.num, right->obj.num);
        return res;
    }
    if(op == KEY_DIV)
    {
        dorl_object* res = dorl_object_create(OBJ_NUM);
        char32_t* temp = str_from_big_num(right->obj.num);
        if(*temp == U'0' && *(temp+1) == U'\0') 
        {
            LAST_ERROR.type = DIV_ZERO;
            printf("Деление на ноль\n");
            arena_destroy();
            exit(1);
        }
        res->obj.num = div_big(left->obj.num, right->obj.num);
        return res;
    }
    if(op == KEY_FLOOR_DIV)
    {
        dorl_object* res = dorl_object_create(OBJ_NUM);
        char32_t* temp = str_from_big_num(right->obj.num);
        if(*temp == U'0' && *(temp+1) == U'\0') 
        {
            LAST_ERROR.type = DIV_ZERO;
            printf("Деление на ноль\n");
            arena_destroy();
            exit(1);
        }
        res->obj.num = div_int_big(left->obj.num, right->obj.num);
        return res;
    }
    if(op == KEY_MOD)
    {
        dorl_object* res = dorl_object_create(OBJ_NUM);
        char32_t* temp = str_from_big_num(right->obj.num);
        if(*temp == U'0' && *(temp+1) == U'\0') 
        {
            LAST_ERROR.type = DIV_ZERO;
            printf("Деление на ноль\n");
            arena_destroy();
            exit(1);
        }
        res->obj.num = mod_big(left->obj.num, right->obj.num);
        return res;
    }
    printf("Неизвестный оператор %d для цифр\n", op);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
bin_str_str(dorl_object* left, dorl_object* right, op_type op)
{
    if(op == KEY_PLUS)
    {
        string* res = str_init(left->obj.str->_str);
        str_cancat(res, right->obj.str->_str);
        dorl_object* r = dorl_object_create(OBJ_STR);
        r->obj.str = res;
        return r;
    }
    if(op == KEY_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(str_equal_fast(left->obj.str, right->obj.str)) res->obj._bool = True;
        else res->obj._bool = False;
        return res;
    }
    if(op == KEY_NOT_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(str_equal_fast(left->obj.str, right->obj.str)) res->obj._bool = False;
        else res->obj._bool = True;
        return res;     
    }
    printf("Неизвестный оператор %d для строк\n", op);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
bin_list_list(dorl_object* left, dorl_object* right, op_type op)
{
    if(op == KEY_PLUS)
    {
        dorl_object* res = dorl_object_create(OBJ_LIST);
        left = dorl_object_copy(left);
        right = dorl_object_copy(right);
        res->obj.root = dk_merge(left->obj.root, right->obj.root);
        return res;
    }
    printf("Неизвестный оператор %d для списков\n", op);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
bin_bool_bool(dorl_object* left, dorl_object* right, op_type op)
{
    if(op == KEY_OR)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(left->obj._bool || right->obj._bool) res->obj._bool = True;
        else res->obj._bool = False;
        return res;
    }
    if(op == KEY_AND)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(left->obj._bool == True && right->obj._bool == True) res->obj._bool = True;
        else res->obj._bool = False;
        return res;
    }
    if(op == KEY_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(left->obj._bool == right->obj._bool) res->obj._bool = True;
        else res->obj._bool = False;
        return res;
    }
    if(op == KEY_NOT_EQUAL)
    {
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(left->obj._bool != right->obj._bool) res->obj._bool = True;
        else res->obj._bool = False;
        return res;
    }
    printf("Неизвестный оператор %d для булей\n", op);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
bin_str_num(dorl_object* left, dorl_object* right, op_type op)
{
    if(op == KEY_MULT)
    {
        dorl_object* res = dorl_object_create(OBJ_STR);
        string* temp = str_init(U"");
        for(size_t i=0;i<szt_from_big_num(right->obj.num);++i)
        {
            str_cancat(temp, str_init(left->obj.str->_str)->_str);
        }
        res->obj.str = temp;
        return res;
    }
    printf("Неизвестный оператор %d для строка число\n", op);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
bin_list_num(dorl_object* left, dorl_object* right, op_type op)
{
    if(op == KEY_MULT)
    {
        dorl_object* res = dorl_object_create(OBJ_LIST);
        dk_node* temp = NULL;
        for(size_t i=0;i<szt_from_big_num(right->obj.num);++i)
        {
            temp = dk_merge(temp, dorl_object_copy(left)->obj.root);
        }
        res->obj.root = temp;
        return res;
    }
    printf("Неизвестный оператор %d для строка число\n", op);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
interpretate_bin(expression* expr)
{
    op_type op = expr->expr.binary->type;
    if(op == KEY_ASSIGN) return bin_assign(expr->expr.binary->left, expr->expr.binary->right);
    dorl_object* left = interpretate_atom(expr->expr.binary->left);
    if(!left) return NULL;
    dorl_object* right = interpretate_atom(expr->expr.binary->right);
    if(!right) return NULL;
    if(left->type == right->type)
    {
        if(left->type == OBJ_NUM) return bin_num_num(left, right, op);
        if(left->type == OBJ_STR) return bin_str_str(left, right, op);
        if(left->type == OBJ_LIST) return bin_list_list(left, right, op);
        if(left->type == OBJ_BOOL) return bin_bool_bool(left, right, op);
    }
    if((left->type == OBJ_NUM || left->type == OBJ_STR) && (right->type == OBJ_NUM || right->type == OBJ_STR))
    {
        if(left->type == OBJ_NUM) return bin_str_num(right, left, op);
        else return bin_str_num(left, right, op);
    }
    if((left->type == OBJ_NUM || left->type == OBJ_LIST) && (right->type == OBJ_NUM || right->type == OBJ_LIST))
    {
        if(left->type == OBJ_NUM) return bin_list_num(right, left, op);
        else return bin_list_num(left, right, op);
    }

    printf("Нет операций %d между %d и %d\n", op, left->type, right->type);
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
interpretate_string(expression* expr)
{
    string* value = expr->expr._string->value;
    dorl_object* res = dorl_object_create(OBJ_STR);
    res->obj.str = value;
    return res;
}

static dorl_object*
interpretate_num(expression* expr)
{
    dorl_object* res = dorl_object_create(OBJ_NUM);
    res->obj.num = big_num_from_str(expr->expr.number->value->_str);
    return res;
}

static dorl_object*
interpretate_list(expression* expr)
{
    dorl_object* res = dorl_object_create(OBJ_LIST);
    link_node* list = expr->expr.list->list;
    while(list)
    {
        res->obj.root = dk_merge(res->obj.root, dk_new_node(dorl_object_copy(interpretate_atom((expression*)list->value))));
        list = list->next;
    }
    return res;
}

static dorl_object*
interpretate_index(expression* expr)
{
    dorl_object* dest = interpretate_atom(expr->expr.index->dest);
    dorl_object* index = interpretate_atom(expr->expr.index->index);
    size_t pos = get_real_index(index, dest);
    if(dest->type == OBJ_LIST)
    {
        return (dorl_object*)dk_get_el(dest->obj.root, pos);
    }
    else
    {
        dorl_object* res = dorl_object_create(OBJ_STR);
        string* str = str_init(U"");
        str_push_c(str,dest->obj.str->_str[pos]);
        res->obj.str = str;
        return res;
    }
    return NULL;
}

static dorl_object*
interpretate_bool(expression* expr)
{
    dorl_object* res = dorl_object_create(OBJ_BOOL);
    res->obj._bool = expr->expr._bool->is_true;
    return res;
}

static dorl_object*
interpretate_un(expression* expr)
{
    dorl_object* obj = interpretate_atom(expr->expr.unary->expr);
    if(expr->expr.unary->type == KEY_MINUS)
    {
        if(obj->type != OBJ_NUM)
        {
            LAST_ERROR.pos_start = expr->_pos;
            LAST_ERROR.type = MISS_NUM;
            return NULL;
        }
        dorl_object* res = dorl_object_create(OBJ_NUM);
        res->obj.num = big_num_from_str(str_from_big_num(obj->obj.num));
        
        if(obj->obj.num->is_negative) res->obj.num->is_negative = 0;
        else res->obj.num->is_negative = 1;
        return res;
    }
    if(expr->expr.unary->type == KEY_NOT)
    {
        if(obj->type != OBJ_BOOL)
        {
            LAST_ERROR.pos_start = expr->_pos;
            LAST_ERROR.type = MISS_BOOL;
            return NULL;
        }  
        dorl_object* res = dorl_object_create(OBJ_BOOL);
        if(obj->obj._bool) res->obj._bool = False;
        else res->obj._bool = True;
        return res;
    }
    printf("Интерпретатор: неизвестный оператор\n");
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
interpretate_if(expression* expr)
{
    dorl_object* cond = interpretate_atom(expr->expr._if->cond);
    if(cond == NULL) return NULL;
    if(cond->type != OBJ_BOOL)
    {
        LAST_ERROR.pos_start = expr->_pos;
        LAST_ERROR.type = MISS_BOOL;
        return NULL;
    }
    if(cond->obj._bool) return interpretate_atom(expr->expr._if->then);
    if(expr->expr._if->_else) return interpretate_atom(expr->expr._if->_else);
    return void_obj;
}

static dorl_object*
interpretate_func(expression* expr)
{
    expression* call = expr->expr.func->call;
    if(call->type != EXPR_CALL)
    {
        printf("Интерпретатор, неправильная функция\n");
        arena_destroy();
        exit(1);
    }
    dorl_object* res = dorl_object_create(OBJ_FUNC);
    res->obj.func->is_dorl = True;
    res->obj.func->src.expr = expr->expr.func->body;
    link_node* vars = NULL;
    size_t cnt = 0;
    link_node* list = call->expr.call->argc;
    while(list != NULL)
    {
        expression* mb_var = (expression*)list->value;
        if(mb_var->type != EXPR_VAR)
        {
            printf("Интерпретатор, неправильная функция\n");
            arena_destroy();
            exit(1);
        }
        vars = link_push_back(vars, link_new(mb_var->expr.var->value));
        list = list->next;
        cnt++;
    }
    vars = link_go_start(vars);
    res->obj.func->cnt = cnt;
    res->obj.func->args = vars;

    if(call->expr.call->dest->type != EXPR_VAR)
    {
        printf("Интерпретатор, неправильная функция\n");
        arena_destroy();
        exit(1);
    }  
    string* name = call->expr.call->dest->expr.var->value;
    environment_change(name, res, CURRENT_ENVI);
    return res;
}

static dorl_object*
interpretate_call(expression* expr)
{
    if(expr->expr.call->dest->type != EXPR_VAR)
    {
        printf("Интерпретатор, неправильный вызов\n");
        arena_destroy();
        exit(1);
    }
    string* name = expr->expr.call->dest->expr.var->value;
    dorl_object* func = environment_get(name, CURRENT_ENVI);
    if(!func)
    {
        printf("Нет функции для вызова\n");
        arena_destroy();
        exit(1);
    }
    size_t args_cnt = 0;
    link_node* list = expr->expr.call->argc;
    link_node* objs = NULL;
    while(list != NULL)
    {
        args_cnt++;
        objs = link_push_back(objs, link_new(dorl_object_get(interpretate_atom((expression*)list->value))));
        list = list->next;
    }
    objs = link_go_start(objs);
    list = link_go_start(list);
    if(args_cnt != func->obj.func->cnt)
    {
        printf("Неправильное количество аргументов в вызове\n");
        arena_destroy();
        exit(1); 
    }
    if(func->obj.func->is_dorl)
    {
        environment* global = CURRENT_ENVI;
        CURRENT_ENVI = environment_create(CURRENT_ENVI);
        link_node* names = func->obj.func->args;
        while(names != NULL)
        {
            string* name = names->value;
            dorl_object* obj = objs->value;
            environment_change(name, obj, CURRENT_ENVI);
            names = names->next;
            objs = objs->next;
        }
        dorl_object* res = interpretate_atom(func->obj.func->src.expr);
        CURRENT_ENVI = global;
        if(res->type == OBJ_RETURN) return dorl_object_copy(res->obj.ret_value);
        return dorl_object_copy(res);
    }

    return func->obj.func->src.c_func(objs);
}

static dorl_object*
interpretate_while(expression* expr)
{
    dorl_object* res = void_obj;
    while(1)
    {
        dorl_object* cond = interpretate_atom(expr->expr._while->cond);
        if(cond->type != OBJ_BOOL)
        {
            printf("Условие в цикле должно быть типа BOOL\n");
            arena_destroy();
            exit(1);
        }
        if(!cond->obj._bool) break;
        res = interpretate_atom(expr->expr._while->body);
        if(res->type == OBJ_RETURN) return res;
        if(res->type == OBJ_INSTRYCTION)
        {
            if(res->obj.instr_type == KEY_BREAK)
            {
                res = void_obj;
                break;
            }
            if(res->obj.instr_type == KEY_CONTINUE)
            {
                res = void_obj;
                continue;
            }
            res = void_obj;
            printf("Неизвестная инструкция в цикле\n");
            arena_destroy();
            exit(1);
        }
    }
    return res;
}

static dorl_object*
interpretate_instryction(expression* expr)
{
    dorl_object* res = dorl_object_create(OBJ_INSTRYCTION);
    res->obj.instr_type = expr->expr.instryct->type;
    return res;
}

static dorl_object*
interpretate_foreach(expression* expr)
{
    dorl_object* res = void_obj;
    if(expr->expr._for->iter->type != EXPR_VAR)
    {
        printf("В цикле for ожидалась переменная для итерации\n");
        arena_destroy();
        exit(1);
    }
    string* name = expr->expr._for->iter->expr.var->value;
    dorl_object* iterable = interpretate_atom(expr->expr._for->list);
    if(iterable->type == OBJ_LIST)
    {
        dk_node* list = iterable->obj.root;
        for(size_t i=0; i<dk_get_size(list);++i)
        {
            dorl_object* el = dk_get_el(list, i);
            environment_change(name, el, CURRENT_ENVI);
            res = interpretate_atom(expr->expr._for->body);
            if(res->type == OBJ_RETURN) return res;
            if(res->type == OBJ_INSTRYCTION)
            {
                if(res->obj.instr_type == KEY_BREAK)
                {
                    res = void_obj;
                    break;
                }
                if(res->obj.instr_type == KEY_CONTINUE)
                {
                    res = void_obj;
                    continue;
                }
                res = void_obj;
                printf("Неизвестная инструкция в цикле\n");
                arena_destroy();
                exit(1);
            }
        }
        return res;
    }

    printf("Некорректный тип для цикла\n");
    arena_destroy();
    exit(1);
    return NULL;
}

static dorl_object*
interpretate_return(expression* expr)
{
    dorl_object* res = dorl_object_create(OBJ_RETURN);
    res->obj.ret_value = interpretate_atom(expr->expr._return->expr);
    return res;
}

static dorl_object*
interpretate_class(expression* expr)
{
    dorl_object* res = dorl_object_create(OBJ_CLASS);
    if(expr->expr.class->name->type != EXPR_VAR)
    {
        printf("Некорректное имя объекта\n");
        arena_destroy();
        exit(1);
    }
    string* name = expr->expr.class->name->expr.var->value;
    if(expr->expr.class->derive)
    {
        if(expr->expr.class->derive->type != EXPR_VAR)
        {
            printf("Некорректное имя наследуемого объекта\n");
            arena_destroy();
            exit(1);
        }
        string* derive_name = expr->expr.class->derive->expr.var->value;
        dorl_object* derive = environment_get(derive_name, CURRENT_ENVI);
        if(!derive)
        {
            printf("Не удалось найти наследуемый объекта\n");
            arena_destroy();
            exit(1);
        }
        res->methods = environment_copy(derive->methods);
    }
    environment* global = CURRENT_ENVI;
    CURRENT_ENVI = res->methods;
    interpretate_atom(expr->expr.class->body);
    CURRENT_ENVI = global;
    res->methods->parent = CURRENT_ENVI;
    environment_change(name, res, CURRENT_ENVI);
    return res;
}

static dorl_object*
interpretate_method(expression* expr)
{
    dorl_object* dest = interpretate_atom(expr->expr.method->dest);
    environment* global = CURRENT_ENVI;
    CURRENT_ENVI = dest->methods;
    dorl_object* res = interpretate_atom(expr->expr.method->call);
    CURRENT_ENVI = global;
    return res;
}

static dorl_object* 
interpretate_atom(expression* expr)
{
    switch(expr->type)
    {
        case EXPR_SEQUE:
            return interpretate_seque(expr);
        case EXPR_VAR:
            return interpretate_var(expr);
        case EXPR_STRING:
            return interpretate_string(expr);
        case EXPR_BIN:
            return interpretate_bin(expr);
        case EXPR_NUM:
            return interpretate_num(expr);
        case EXPR_LIST:
            return interpretate_list(expr);
        case EXPR_INDEX:
            return interpretate_index(expr);
        case EXPR_BOOL:
            return interpretate_bool(expr);
        case EXPR_UN:
            return interpretate_un(expr);
        case EXPR_IF:
            return interpretate_if(expr);
        case EXPR_FUNC:
            return interpretate_func(expr);
        case EXPR_CALL:
            return interpretate_call(expr);
        case EXPR_WHILE:
            return interpretate_while(expr);
        case EXPR_INSTRYCT:
            return interpretate_instryction(expr);
        case EXPR_FOREACH:
            return interpretate_foreach(expr);
        case EXPR_RETURN:
            return interpretate_return(expr);
        case EXPR_CLASS:
            return interpretate_class(expr);
        case EXPR_METHOD:
            return interpretate_method(expr);
        default:
            printf("Интерпретатор неизвестное выражение\n");
            arena_destroy();
            exit(1);
    }
    return NULL;
}

dorl_object*
interpretate(expression* AST)
{
    void_obj = dorl_object_create(OBJ_VOID);
    CURRENT_ENVI = environment_create(NULL);
    set_global();
    interpretate_atom(AST);
    return void_obj;
}