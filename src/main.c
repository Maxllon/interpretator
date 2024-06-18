#include<stdio.h>
#include<stdlib.h>

#include"includes.h"
#include"read_file.h"
#include"lexer.h"
#include"parcer.h"
#include"arena.h"

int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE,".UTF8");
    Arena* ARENA = arena_create();

    char* filename = "example";
    wchar* file;
    read_file(filename, &file, ARENA);
    if(out_file) wprintf(L"%ls\n\n\n\n",file);

    tk_node* main = lexing(file);

    struct Expretion* expr = parce(main);
    
    delete_tk_list(main);
    delete_expr(expr);
    arena_destroy(ARENA);
    system("pause");
    return 0;
}