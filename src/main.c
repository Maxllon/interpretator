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

    wchar* filename = L"example.txt";
    wchar* file;
    read_file_win(L"example.txt", &file, ARENA);
    if(out_file) wprintf(L"%ls\n\n\n\n",file);
    tk_node* main = lexing(file, ARENA);
    Expression* expr = parse(main, ARENA);

    big_num* a = big_num_from_str(L"412412141251251251251251", ARENA);
    big_num* b = big_num_from_str(L"9", ARENA);
    big_num* c = div_big(a, b, ARENA);
    wchar* str = str_from_big_num(c, ARENA);
    wprintf(L"%ls\n", str);

/*   interpretate(expr, ARENA);
*/
    arena_destroy(ARENA);
    system("pause");
    return 0;
}