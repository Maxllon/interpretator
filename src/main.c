#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#include "read_file.h"
#include "lexer.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    wchar* file = read_file("example");
    
    wchar* f = file;
    wprintf(L"%ls\n", f);

    tk_node* main = lexing(file);

    delete_tk_list(main);
    free(file);
    system("pause");
    return 0;
}