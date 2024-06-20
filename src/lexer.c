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
        wprintf(L"Тип токена: %s\n", TOKEN_NAMES[token->kind]);
        wprintf(L"Значение токена: %ls\n", token->value);
        wprintf(L"Позиция токена: %d %d\n\n", token->pos.x, token->pos.y);  
        ++i;
    }
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

const wchar* KEYWORDS[] = {L"коли", L"доселе", L"инако", L"обрати", L"не", L"бери", L"из", L"служ", L"нч", L"кц", L"БЫЛЬ", L"БЛЯДЬ", L"то", L"НЕЧА", L"рвать"};
const wchar* BIN_OPS[] = {L"и", L"або"};

const wchar* BIN_OP = L"+-=*/^>!<\%";
const wchar* DIGITS = L"0123456789.";
const wchar* SPEC = L";,()[]{}:\"";

tk_node* lexing(wchar* file, Arena* arena)
{
    ARENA = arena;
    
    TOKEN_NAMES[END] = L"КОНЕЦ";
    TOKEN_NAMES[START] = L"НАЧАЛО";
    TOKEN_NAMES[KEYWORD] = L"СЛОВО-КЛЮЧ";
    TOKEN_NAMES[VARIABLE] = L"ПЕРЕМЕННАЯ";
    TOKEN_NAMES[STRING] = L"СТРОКА";
    TOKEN_NAMES[BINARY] = L"ОПЕРАТОР";
    TOKEN_NAMES[NUMBER] = L"ЧИСЛО";

    TOKEN_NAMES[SYMBOL] = L"СИМВОЛ";
    TOKEN_NAMES[SPECIAL] = L"ЗНАК";
    TOKEN_NAMES[DIGIT] = L"ЦИФРА";
    TOKEN_NAMES[BIN] = L"ОПЕРАТОР";


    //--------------------------lexing symbols-------------------------
    VEC_2 pos = {-1,-1};
    tk_node* symbols = new_node(START, NULL,pos);
    pos.x = 0;
    pos.y = 0;
    wchar* str;
    wchar sym;
    for(size_t i = 0; i < wcslen(file); ++i)
    {
        sym = *(file+i);
        if(wcschr(SPEC, sym) != NULL)
        {
            emp_str(str);
            push_node(symbols, new_node(SPECIAL, str, pos));
        }
        else if(sym == L'\n')
        {
            pos.y++;
            pos.x = 0;
            emp_str(str);
            *str = L' ';
            push_node(symbols, new_node(SYMBOL, str, pos));
        }
        else if(wcschr(BIN_OP, sym) != NULL)
        {
            emp_str(str);
            push_node(symbols, new_node(BINARY, str, pos));
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
        if(sym != L'\n') pos.x++;
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
            
            push_node(main, new_node(NUMBER, str, pos));
        }

        //lex special symbols
        else if(symbols->kind == SPECIAL)
        {
            if(*(symbols->value) == L'\"')
            {
                if(symbols->next->kind == END)
                {;
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
        //lex words
        else if(symbols->kind == SYMBOL && *(symbols->value) != L' ')
        {
            while(symbols->kind == SYMBOL && *(symbols->value) != L' ')
            {
                bm_wcscat(str, symbols->value);
                symbols = symbols->next;
            }
            for(size_t i = 0; i < sizeof(KEYWORDS)/sizeof(wchar*); ++i)
            {
                if(wcscmp(str, KEYWORDS[i]) == 0)
                {
                    push_node(main, new_node(KEYWORD, str, pos));
                    temp++;
                }
            }
            if(!temp)
            {
            for(size_t i = 0; i < sizeof(BIN_OPS)/sizeof(wchar*); ++i)
            {
                if(wcscmp(str, BIN_OPS[i]) == 0)
                {
                    push_node(main, new_node(BINARY, str, pos));
                    temp++;
                }
                
            }
            }
            if(!temp) push_node(main, new_node(VARIABLE, str, pos));


            symbols = symbols->previous;
        }
        //lex bin
        else if(symbols->kind == BINARY)
        {
            while(symbols->kind == BINARY)
            {
                bm_wcscat(str, symbols->value);
                symbols = symbols->next;
            }
            push_node(main, new_node(BINARY, str, pos));

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