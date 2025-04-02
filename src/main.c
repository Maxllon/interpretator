#include<stdio.h>
#include<stdlib.h>

#include"includes.h"
#include"read_file.h"
#include"lexer.h"
#include"parser.h"
#include"arena.h"
#include"interpretator.h"
#include"long_num.h"
#include"bmpl_string.h"

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

    bmpl_string* str1 = bmpl_string_from_str(L"бананчик", ARENA);
    bmpl_string* str2 = bmpl_string_from_str(L" и арбуз ", ARENA);
    bmpl_string* str3 = bmpl_string_cancat(str1, str2, ARENA);
    big_num* a = big_num_from_str(L"0", ARENA);
    big_num* b = big_num_from_str(L"8", ARENA);
    big_num* c = big_num_from_str(L"7", ARENA);
    bmpl_string* str4 = bmpl_string_mult(str3, b, ARENA);
    bmpl_string* str5 = slice(str4, a, b, ARENA);
    bmpl_string* str6 = bmpl_string_copy(str5, ARENA);
    bmpl_string* str7 = slice(str1,a, c,ARENA );
    bmpl_string* str9 = bmpl_string_from_str(L"в ", ARENA);
    bmpl_string* str8 = bmpl_string_cancat(str7, str9, ARENA);
    wprintf(L"%ls\n", str6->string);
    wprintf(L"%ls\n", str8->string);
    wprintf(L"%d\n", bmpl_string_equal(str6, str8));


/*   interpretate(expr, ARENA);
*/
    arena_destroy(ARENA);
    system("pause");
    return 0;
}