#include<token.h>
#include<globals.h>
#include<my_link_list.h>

void
out_symb_list(link_node* root)
{
    if(root == NULL) return;
    string* out = str_init(U"");
    root = root->next;
    printf("\n\n");
    while(((symb_token*)root->value)->type != (symb_type)EOF)
    {
        switch(((symb_token*)root->value)->type)
        {
            case DIGIT:
                printf("Цифра");
                break;
            case SEP:
                printf("Знак");
                break;
            case OP:
                printf("Оператор");
                break;
            case LETTER:
                printf("Буква");
                break;
            case WHITE:
                printf("Пробел");
                break;
            default:
                printf("Неизвестный тип в выводе токена\n");
                arena_destroy();
                exit(1);
                break;
        }
        printf(": ");
        str_clear(out);
        str_push_c(out, ((symb_token*)root->value)->c);
        str_out(out);
        printf(" <%lu, %lu>\n", ((symb_token*)root->value)->_pos.x, ((symb_token*)root->value)->_pos.y);
        root = root->next;
    }
    printf("\n");
}

void
out_tk_list(link_node* root)
{
    root = root->next;
    printf("\n\n");
    while(((token*)root->value)->type != KEY_END)
    {
        switch(((token*)root->value)->type)
        {
            case KEY_STRING:
                printf("Строка: \"");
                break;
            case KEY_VARIABLE:
                printf("Переменная: ");
                break;
            case KEY_OP:
                printf("Оператор: ");
                break;
            case KEY_KEYWORD:
                printf("Ключевое слово: ");
                break;
            case KEY_SEPARATOR:
                printf("Сепаратор: ");
                break;
            case KEY_NUMBER:
                printf("Число: ");
                break;
            case KEY_INSTRYCTION:   
                printf("Инструкция: ");
                break;
            case KEY_BOOL:
                printf("Логический: ");
                break;
            default:
                printf("Неизвестный тип токена\n");
                arena_destroy();
                exit(1);
                break;
        }
        str_out(((token*)root->value)->value);
        if(((token*)root->value)->type == KEY_STRING) printf("\"");
        printf(" <%lu, %lu>\n", ((token*)root->value)->_pos.x, ((token*)root->value)->_pos.y);
        root = root->next;
    }
    printf("\n");
}

symb_token*
new_symb_token(char32_t c, symb_type type, pos _pos)
{
    symb_token* tk = arena_alloc(sizeof(symb_token));
    if(tk == NULL) return NULL;
    tk->c = c;
    tk->type = type;
    tk->_pos = _pos;
    return tk;
}

token*
new_token(token_type type, int subtype, string* value, pos _pos)
{
    token* tk = arena_alloc(sizeof(token));
    if(tk == NULL) return NULL;
    tk->type = type;
    tk->subtype = subtype;
    tk->value = value;
    tk->_pos = _pos;
    return tk;
}

static bool_t 
in(char32_t c, char32_t* str)
{
    for(int i = 0; str[i] != U'\0'; ++i)
    {
        if(str[i] == c) return True;
    }
    return False;
}

static link_node*
lex_symbols(string* file)
{
    char32_t* digits = U"0123456789";
    char32_t* separators = U",.;:\"{}[]()";
    char32_t* operators = U"+-=*&^|/%%>!<";

    pos _pos;
    def_pos(_pos, 0, 0);

    link_node* list = NULL;
    list = link_push_back(list, link_new(new_symb_token(U'\0', EOF, _pos)));
    
    char32_t* temp = file->_str;
    
    while(*temp != U'\0')
    {
        if(*temp == U'\n')
        {
            _pos.x = 0;
            _pos.y++;
            list = link_push_back(list, link_new(new_symb_token(U' ', WHITE, _pos)));
        }
        else if(*temp == U' ')
        {
            list = link_push_back(list, link_new(new_symb_token(*temp, WHITE, _pos)));
        }
        else if(in(*temp, digits))
        {
            list = link_push_back(list, link_new(new_symb_token(*temp, DIGIT, _pos)));
        }
        else if(in(*temp, separators))
        {
            list = link_push_back(list, link_new(new_symb_token(*temp, SEP, _pos)));
        }
        else if(in(*temp, operators))
        {
            list = link_push_back(list, link_new(new_symb_token(*temp, OP, _pos)));
        }
        else 
        {
            list = link_push_back(list, link_new(new_symb_token(*temp, LETTER, _pos)));
        }
        _pos.x++;
        temp++;

    }
    list = link_push_back(list, link_new(new_symb_token(*temp, EOF, _pos)));
    list = link_go_start(list);
    return list;
}

static bool_t
skip_white(link_node* root)
{
    bool_t res = False;
    link_node* start = root->pref;
    while(((symb_token*)root->value)->type == WHITE)
    {
        res = True;
        root = root->next;
    }
    if(res) 
    {
        start->next = root;
        root->pref = start;
    }
    return res;
}


static link_node*
get_num_token(link_node* root)
{
    pos _pos = ((symb_token*)root->value)->_pos;
    string* str = str_init(U"");
    if(str == NULL) return NULL;
    link_node* start = root->pref;
    while(((symb_token*)root->value)->type == DIGIT)
    {
        if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
        root = root->next;
    }
    if(str->_str[0] == U'0' && str->len > 1)
    {
        LAST_ERROR.pos_start = _pos;
        LAST_ERROR.type = WRONG_NUM;
        return NULL;
    }
    if(((symb_token*)root->value)->c == U'.')
    {
        if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
        root = root->next;
        if(((symb_token*)root->value)->type != DIGIT)
        {
            LAST_ERROR.pos_start = _pos;
            LAST_ERROR.type = WRONG_NUM;
            return NULL;
        }
        while(((symb_token*)root->value)->type == DIGIT)
        {
            if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
            root = root->next;
        }
    }
    start->next = root;
    root->pref = start;
    return link_new(new_token(KEY_NUMBER, EOF, str, _pos));
}

#define fast_tk(key, str, subtype) link_new(new_token(key, subtype, str_init(str), _pos))

static link_node*
get_sep_token(link_node* root)
{
    pos _pos = ((symb_token*)root->value)->_pos;
    link_node* arr[] = 
    {
        fast_tk(KEY_SEPARATOR, U".", KEY_DOT),
        fast_tk(KEY_SEPARATOR, U",", KEY_COMMA),
        fast_tk(KEY_SEPARATOR, U":", KEY_COLON),
        fast_tk(KEY_SEPARATOR, U";", KEY_SEMICOLON),
        fast_tk(KEY_SEPARATOR, U"[", KEY_SQUARE_OPEN),
        fast_tk(KEY_SEPARATOR, U"]", KEY_SQUARE_CLOSE),
        fast_tk(KEY_SEPARATOR, U"(", KEY_PARENTHESIS_OPEN),
        fast_tk(KEY_SEPARATOR, U")", KEY_PARENTHESIS_CLOSE),

        NULL
    };
    string* str = str_init(U"");
    if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
    root->next->pref = root->pref;
    root->pref->next = root->next;
    for(int i = 0; arr[i] != NULL; ++i)
    {
        if(arr[i]->value == NULL) return NULL;
        if(str_equal(str, ((token*)arr[i]->value)->value)) return arr[i];
    }
    LAST_ERROR.pos_start = _pos;
    LAST_ERROR.type = UNKNOWN_SEP;
    return NULL;
}

static link_node*
get_str_token(link_node* root)
{
    pos _pos = ((symb_token*)root->value)->_pos;
    string* str = str_init(U"");
    if(str == NULL) return NULL;
    link_node* start = root->pref;
    root = root->next;
    while(((symb_token*)root->value)->c != U'\"')
    {
         if(((symb_token*)root->value)->type == (symb_type) EOF)
         {
            LAST_ERROR.pos_start = _pos;
            LAST_ERROR.type = STR_WITHOUT_END;
            return NULL;
         }
         if(((symb_token*)root->value)->c == U'\\')
         {
            if(((symb_token*)root->next->value)->type == (symb_type) EOF)
            {
                LAST_ERROR.pos_start = ((symb_token*)root->value)->_pos;
                LAST_ERROR.type = INCORRECT_STRING;
                return NULL;
            }
            root = root->next;
            switch(((symb_token*)root->value)->c)
            {
                case U'\\':
                    if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
                    break;
                case U'н':
                    if(str_push_c(str, U'\n') != ALL_GOOD) return NULL;
                    break;
                case U'n':
                    if(str_push_c(str, U'\n') != ALL_GOOD) return NULL;
                    break;
                default:
                    LAST_ERROR.pos_start = ((symb_token*)root->value)->_pos;
                    LAST_ERROR.type = INCORRECT_STRING;
                    return NULL;  
            }
         }
         else
         {
            if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
         }
         root = root->next;
    }
    root = root->next;
    start->next = root;
    root->pref = start;
    return link_new(new_token(KEY_STRING, EOF, str, _pos));
}

static link_node*
get_word_token(link_node* root)
{
    pos _pos = ((symb_token*)root->value)->_pos;
    link_node* arr[] = 
    {
        fast_tk(KEY_OP, U"и", KEY_AND),
        fast_tk(KEY_OP, U"и_бит", KEY_AND_BIT),
        fast_tk(KEY_OP, U"или", KEY_OR),
        fast_tk(KEY_OP, U"или_бит", KEY_OR_BIT),
        fast_tk(KEY_OP, U"ксор", KEY_XOR),
        fast_tk(KEY_OP, U"ксор_бит", KEY_XOR_BIT),
        fast_tk(KEY_OP, U"не", KEY_NOT),
        fast_tk(KEY_OP, U"не_бит", KEY_NOT_BIT),

        fast_tk(KEY_KEYWORD, U"пока", KEY_WHILE),
        fast_tk(KEY_KEYWORD, U"для", KEY_FOREACH),
        fast_tk(KEY_KEYWORD, U"из", KEY_IN),
        fast_tk(KEY_KEYWORD, U"если", KEY_IF),
        fast_tk(KEY_KEYWORD, U"иначе", KEY_ELSE),
        fast_tk(KEY_KEYWORD, U"функ", KEY_FUNC),
        fast_tk(KEY_KEYWORD, U"объект", KEY_CLASS),
        fast_tk(KEY_KEYWORD, U"нч", KEY_START_BLOCK),
        fast_tk(KEY_KEYWORD, U"кц", KEY_END_BLOCK),
        fast_tk(KEY_KEYWORD, U"вернуть", KEY_RETURN),

        fast_tk(KEY_INSTRYCTION, U"продолжить", KEY_CONTINUE),
        fast_tk(KEY_INSTRYCTION, U"прекратить", KEY_BREAK),

        fast_tk(KEY_BOOL, U"Истина", KEY_TRUE),
        fast_tk(KEY_BOOL, U"Ложь", KEY_FALSE),
        NULL
    };
    string* str = str_init(U"");
    link_node* start = root->pref;
    while(((symb_token*)root->value)->type == LETTER || ((symb_token*)root->value)->type == DIGIT)
    {
        if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
        root = root->next;
    }
    start->next = root;
    root->pref = start;
    for(int i = 0; arr[i] != NULL; ++i)
    {
        if(arr[i]->value == NULL) return NULL;
        if(str_equal(str, ((token*)arr[i]->value)->value)) return arr[i];
    }
    return link_new(new_token(KEY_VARIABLE, EOF, str, _pos));
}

static link_node*
get_op_token(link_node* root)
{
    pos _pos = ((symb_token*)root->value)->_pos;
    link_node* arr[] = 
    {
        fast_tk(KEY_OP, U"+", KEY_PLUS),
        fast_tk(KEY_OP, U"-", KEY_MINUS),
        fast_tk(KEY_OP, U"*", KEY_MULT),
        fast_tk(KEY_OP, U"/", KEY_DIV),
        fast_tk(KEY_OP, U"//", KEY_FLOOR_DIV),
        fast_tk(KEY_OP, U"^", KEY_POW),
        fast_tk(KEY_OP, U"%", KEY_MOD),
        fast_tk(KEY_OP, U"=", KEY_ASSIGN),
        fast_tk(KEY_OP, U"==", KEY_EQUAL),
        fast_tk(KEY_OP, U"!=", KEY_NOT_EQUAL),
        fast_tk(KEY_OP, U"==", KEY_EQUAL),
        fast_tk(KEY_OP, U">", KEY_GREATER),
        fast_tk(KEY_OP, U"<", KEY_LESS),
        fast_tk(KEY_OP, U">=", KEY_GREATER_EQUAL),
        fast_tk(KEY_OP, U"<=", KEY_LESS_EQUAL),
        NULL
    };

    string* str = str_init(U"");
    link_node* start = root->pref;
    while(((symb_token*)root->value)->type == OP)
    {
        if(str_push_c(str, ((symb_token*)root->value)->c) != ALL_GOOD) return NULL;
        root = root->next;
    }
    start->next = root;
    root->pref = start;
    for(int i = 0; arr[i] != NULL; ++i)
    {
        if(arr[i]->value == NULL) return NULL;
        if(str_equal(str, ((token*)arr[i]->value)->value)) return arr[i];
    }
    LAST_ERROR.pos_start = _pos;
    LAST_ERROR.type = UNKNOWN_OP;
    return NULL;
}
#undef fast_tk

static link_node*
get_token(link_node* root)
{
    switch(((symb_token*)root->value)->type)
    {
        case LETTER:
            return get_word_token(root);
        case OP:
            return get_op_token(root);
        case SEP:
            if(((symb_token*)root->value)->c == '\"') return get_str_token(root);
            return get_sep_token(root);
        case DIGIT:
            return get_num_token(root);
        default:
            printf("Неизвестный тип токена в get_token %d\n", ((symb_token*)root->value)->type);
            arena_destroy();
            exit(1);
            break;
    }
    return NULL;
}

link_node*
tokenize(string* file)
{
    link_node* root = lex_symbols(file);
    if(root == NULL) return NULL;
    if(OUT_SYMB_LIST) out_symb_list(root);
    pos _pos;
    def_pos(_pos, 0, 0);
    link_node* tk_list = link_new(new_token(KEY_START, EOF, NULL, _pos));
    link_node* temp = tk_list;
    while(((symb_token*)root->next->value)->type != (symb_type) EOF)
    {
        if(skip_white(root->next)) continue;
        temp = link_push_back(temp, get_token(root->next));
        if(LAST_ERROR.type != ALL_GOOD) return NULL;
    }

    temp = link_push_back(temp, link_new(new_token(KEY_END, EOF, NULL, _pos)));

    return tk_list;
}