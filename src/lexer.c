#include"lexer.h"
tk_node* new_node(TOKEN_KIND kind, wchar* value, VEC_2 pos)
{
    tk_node* node = malloc(sizeof(tk_node));
    node->kind = kind;
    node->next = NULL;
    node->previous = NULL;
    node->pos = pos;
    node->value = value;
    return node;
}

void delete_tk_list(tk_node* main)
{
    if(main == NULL) return;
    if(main->next != NULL) delete_tk_list(main->next);
    bm_free(main->value);
    bm_free(main);
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
const char* name_tokens[] = {"END", "START", "KEYWORD", "VARIABLE", "STRING", "BINARY", "SPECIAL", "DIGIT", "PLAIN", "SYMBOL", "NUMBER", "STD"};
void out_tk_list(tk_node *main)
{
    while(1)
    {
        printf("kind: %s ", name_tokens[main->kind]);
        wprintf(L"value: %ls ", main->value);
        printf("pos: %d %d\n", main->pos.x, main->pos.y);
        if(main->kind == END) break;
        main = main->next;
    }
    printf("\n\n\n");
}
int count(wchar* str, wchar symb)
{
    int c = 0;
    while((str = wcschr(str, symb)) != NULL)
    {
        c++;
        str++;
    }
    return c;
}

const wchar* KEYWORDS[] = {L"коли", L"доселе", L"инако", L"обрати", L"не", L"вернуть", L"из", L"служ", L"нч", L"кн", L"БЫЛЬ", L"БЛЯДЬ"};
const wchar* STDFUNC[] = {L"покажи", L"взимать", L"межа"};
const wchar* BIN_OP = L"+-=*/^>!<\%";
const wchar* BIN_OPS[] = {L"и", L"або"};

const wchar* DIGITS = L"0123456789.";
const wchar* SPEC = L";,()[]\"";

tk_node* lexing(wchar* file)
{
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
        str = malloc(2);
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
                delete_tk_list(main);
                delete_tk_list(symbols);
                bm_free(file);
                bm_free(str);
                error("this number cant start with zero",pos);
            }
            if(count(str, L'.') > 1)
            {
                delete_tk_list(main);
                delete_tk_list(symbols);
                bm_free(file);
                bm_free(str);
                error("this number has more than 1 dot", pos);
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
                    delete_tk_list(main);
                    delete_tk_list(symbols);
                    bm_free(str);
                    error("there is no end for the string", pos);
                }
                while(*((symbols = symbols->next)->value) != L'\"')
                {
                    if(symbols->next->kind == END)
                    {
                        delete_tk_list(main);
                        delete_tk_list(symbols);
                        bm_free(str);
                        error("there is no end of the string", pos);
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
            for(size_t i = 0; i < sizeof(STDFUNC)/sizeof(wchar*); ++i)
            {
                if(wcscmp(str, STDFUNC[i]) == 0)
                {
                    push_node(main, new_node(STD, str, pos));
                    temp++;
                }
                
            }
            }
            if(!temp)
            {
            for(size_t i = 0; i < sizeof(BIN_OPS)/sizeof(wchar*); ++i)
            {
                if(wcscmp(str, BIN_OPS[i]) == 0)
                {
                    push_node(main, new_node(VARIABLE, str, pos));
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
    delete_tk_list(symbols);
    if(out_tklist) out_tk_list(main);
    return main;
}