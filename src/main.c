#include<stdio.h>
#include<stdlib.h>

#include"includes.h"
#include"read_file.h"
#include"lexer.h"
#include"parser.h"
#include"arena.h"
#include"interpretator.h"
#include"long_num.h"

int main(int argc, char *argv[])
{

    _wsetlocale(LC_ALL, L"");
    Arena* ARENA = arena_create();
    char* a = arena_alloc(ARENA, 10003);
    char* b = arena_alloc(ARENA, 10003);
    /*wchar* filename = L"example.txt";
    wchar* file;
    read_file_win(L"example.txt", &file, ARENA);
    if(out_file) wprintf(L"%ls\n\n\n\n",file);
    tk_node* main = lexing(file, ARENA);
    Expression* expr = parse(main, ARENA);
    /*big_num* a = big_num_from_str(L"-2472217491224172572715712957175174912241725727157129571759275971295724722174912749117491224172572715712957175927597129572472217491274919275971295724722174912749182749128745981725792719571295717592759712957749182749128745981725792719571295717592759712957.1", ARENA);
    wprintf(L"%ls\n", str_from_big_num(a, ARENA));
    /*big_num* b = big_num_from_str(L"-5", ARENA);
    big_num* c = sub_big(a, b, ARENA);
    wchar* str = str_from_big_num(c, ARENA);
    wprintf(L"%ls\n", str);*/

/*   interpretate(expr, ARENA);
*/
    arena_destroy(ARENA);
    system("pause");
    return 0;
}