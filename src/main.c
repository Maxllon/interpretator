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
#include"dekart_tree.h"

int main(int argc, char *argv[])
{

    _wsetlocale(LC_ALL, L"");
    srand(time(NULL));
    Arena* ARENA = arena_create();
    wchar* filename = L"example.txt";
    wchar* file;
    read_file_win(L"example.txt", &file, ARENA);
    if(out_file) wprintf(L"%ls\n\n\n\n",file);
    tk_node* main = lexing(file, ARENA);
    Expression* expr = parse(main, ARENA);


    dk_node* root = NULL;
    for(size_t i=1;i<=15;++i)
    {
        size_t* h = arena_alloc(ARENA, sizeof(size_t));
        *h = i;
        dk_node* R = dk_new_node(h, ARENA);
        root = dk_merge(root, R);
    }
    size_t* p = arena_alloc(ARENA, sizeof(size_t));
    *p = 412;
    dk_node* res = dk_add(root, dk_new_node(p, ARENA), 3);
    dk_output(res);
    wprintf(L"\n");
    res = dk_erase(res, 4);
    dk_output(res);
    wprintf(L"\n");
    p = dk_get_el(res, 8);
    wprintf(L"%llu\n", *p);
/*   interpretate(expr, ARENA);
*/
    arena_destroy(ARENA);
    system("pause");
    return 0;
}