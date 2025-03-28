#include"lexer.h"
static Arena* ARENA = NULL;

tk_node* new_node(TOKEN_KIND kind, wchar* value, VEC_2 pos)
{
    tk_node* node = arena_alloc(ARENA, sizeof(tk_node));
    node->kind = kind;
    node->next = NULL;
    node->previous = NULL;
    node->pos = pos;
    node->value = value;
    return node;
}

void push_node(tk_node* main, tk_node* node)
{
    tk_node* st = main;
    while(st->next != NULL)
    {
        st = st->next;
    }
    st->next = node;
    node->previous = st;
}

tk_node* go_start(tk_node* main)
{
    while(main->previous != NULL) main = main->previous;
    return main;
}
static wchar* TOKEN_NAMES[sizeof(TOKEN_KIND)];

void out_tk_list(tk_node *main)
{
    size_t i = 1;
    for (tk_node* token = main; token->kind != END; token = token->next)
    {
        wprintf(L"<%d>", i);
        wprintf(L"Тип токена: %ls\n", TOKEN_NAMES[token->kind]);
        wprintf(L"Значение токена: %ls\n", token->value);
        wprintf(L"Позиция токена: %d %d\n\n", token->pos.x, token->pos.y);  
        ++i;
    }
    wprintf(L"<%d>", i);
    wprintf(L"Тип токена: %ls\n", TOKEN_NAMES[END]);
    wprintf(L"Значение токена: %ls\n", NULL);
    wprintf(L"Позиция токена: %d %d\n\n", -1, -1);  
    printf("\n\n\n");
}
static size_t count(wchar* str, wchar symb)
{
    size_t c = 0;
    while((str = wcschr(str, symb)) != NULL)
    {
        c++;
        str++;
    }
    return c;
}

static const wchar* DIGITS = L"0123456789.";
static const wchar* OPERATORS = L"+-=><*/^\%";
static const wchar* SPECIALS = L"(){}[];\",";

static const wchar* KEYWORDS[] =
{
    L"пока",
    L"для",
    L"функ",
    L"вернуть",
    L"закончить",
    L"продожить",
    L"НЕ",
    NULL
};
static const wchar* BIN_OPERATORS[] =
{
    L"+",
    L"-",
    L"*",
    L"/",
    L"\%",
    L"^",
    L"ИЛИ",
    L"И",
    L"==",
    L"=",
    L">=",
    L"<=",
    L">",
    L"<",
    NULL
};

tk_node* lexing(wchar* file, Arena* arena)
{
    if(file==NULL)
    {
        return NULL;
    }
    ARENA = arena;
    
    TOKEN_NAMES[END] = L"КОНЕЦ";
    TOKEN_NAMES[START] = L"НАЧАЛО";
    TOKEN_NAMES[KEYWORD] = L"СЛОВО-КЛЮЧ";
    TOKEN_NAMES[VARIABLE] = L"ПЕРЕМЕННАЯ";
    TOKEN_NAMES[STRING] = L"СТРОКА";
    TOKEN_NAMES[BINARY] = L"БИНАРНЫЙ ОПЕРАТОР";
    TOKEN_NAMES[NUMBER] = L"ЧИСЛО";

    TOKEN_NAMES[SYMBOL] = L"СИМВОЛ";
    TOKEN_NAMES[SPECIAL] = L"ЗНАК";
    TOKEN_NAMES[DIGIT] = L"ЦИФРА";
    TOKEN_NAMES[OP] = L"ОПЕРАТОР";


    //--------------------------lexing symbols-------------------------
    VEC_2 pos = {-1,-1};
    tk_node* symbols = new_node(START, NULL,pos);
    pos.x = 0;
    pos.y = 0;
    wchar* str;
    wchar sym;
    for(size_t i = 0; file[i]; ++i)
    {
        sym = file[i];
        if(sym == L'\r') continue;
        if(sym == L'\n')
        {
            pos.y++;
            pos.x = 0;
            continue;
        }
        if(wcschr(SPECIALS, sym) != NULL)
        {
            emp_str(str);
            push_node(symbols, new_node(SPECIAL, str, pos));
        }
        else if(wcschr(OPERATORS, sym) != NULL)
        {
            emp_str(str);
            push_node(symbols, new_node(OP, str, pos));
        }
        else if(wcschr(DIGITS, sym) != NULL)
        {
            emp_str(str);
            push_node(symbols, new_node(DIGIT, str, pos));
        }
        else
        {
            emp_str(str);
            push_node(symbols, new_node(SYMBOL, str, pos));          
        }
        pos.x++;
    }
    pos.x = -1;
    pos.y = -1;
    push_node(symbols, new_node(END, NULL, pos));
    if(out_symbols) out_tk_list(symbols);
    //----------------------end lexing symbols-------------------------
    pos.x = -1;
    pos.y = -1;
    tk_node* main = new_node(START, NULL,pos);
    int temp;
    while(symbols->kind != END)
    {
        temp = 0;
        str = arena_alloc(ARENA, 2);
        *str = L'\0';
        pos = symbols->pos;
        //lex number
        if(symbols->kind == DIGIT)
        {
            while(symbols->kind == DIGIT)
            {
                bm_wcscat(str, symbols->value);
                symbols = symbols->next;
            }
            symbols = symbols->previous;

            if(*str == L'0' && *(str+1) != L'.' && *(str+1) != L'\0')
            {
                wprintf(L"Ошибка: Это число не может начинаться с нуля!! <%d><%d>\n", pos.x, pos.y);
                EXIT;
            }
            if(count(str, L'.') > 1)
            {
                wprintf(L"Ошибка: В числе не может быть две точки!! <%d><%d>\n", pos.x, pos.y);
                EXIT;
            }
            wchar* temp = str;
            while(*(temp) != L'\0') temp++;
            if(*(temp-1) == L'.')
            {
                wprintf(L"Ошибка: Число не должно заканчиваться точкой!! <%d><%d>\n", pos.x, pos.y);
                EXIT;
            }
            
            push_node(main, new_node(NUMBER, str, pos));
        }

        //lex special symbols
        else if(symbols->kind == SPECIAL)
        {
            if(*(symbols->value) == L'\"')
            {
                if(symbols->next->kind == END)
                {
                    wprintf(L"Ошибка: строка не имеет конца!! <%d><%d>\n", pos.x, pos.y);
                    EXIT;
                }
                while(*((symbols = symbols->next)->value) != L'\"')
                {
                    if(symbols->next->kind == END)
                    {
                        wprintf(L"Ошибка: строка не имеет конца!! <%d><%d>\n", pos.x, pos.y);
                        EXIT;
                    }
                    bm_wcscat(str, symbols->value);
                }
                push_node(main, new_node(STRING, str, pos));
            }
            else
            {
                bm_wcscat(str, symbols->value);
                push_node(main, new_node(SPECIAL, str, pos));
            }
        }
        //lex operators
        else if(symbols->kind == OP && *(symbols->value) != L' ')
        {
            while(symbols->kind == OP && *(symbols->value) != L' ')
            {
                bm_wcscat(str, symbols->value);
                symbols = symbols->next;
            }
            for(size_t i = 0; BIN_OPERATORS[i]; ++i)
            {
                if(wcscmp(str, BIN_OPERATORS[i]) == 0)
                {
                    push_node(main, new_node(BINARY, str, pos));
                }
            }
            symbols = symbols->previous;
        }
        //lex words
        else if(symbols->kind == SYMBOL && *(symbols->value) != L' ')
        {
            while(symbols->kind == SYMBOL && *(symbols->value) != L' ')
            {
                bm_wcscat(str, symbols->value);
                symbols = symbols->next;
            }
            for(size_t i = 0; KEYWORDS[i]; ++i)
            {
                if(wcscmp(str, KEYWORDS[i]) == 0)
                {
                    push_node(main, new_node(KEYWORD, str, pos));
                    temp++;
                }
            }
            if(!temp)
            {
                push_node(main, new_node(VARIABLE, str, pos));
            }


            symbols = symbols->previous;
        }
        
        symbols = symbols->next;
    }
    pos.x = -1;
    pos.y = -1;
    push_node(main, new_node(END, NULL, pos));
    symbols = go_start(symbols);
    if(out_tklist) out_tk_list(main);
    return main;
}