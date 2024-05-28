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

    VEC_2 pos = {0,1};
    tk_node* main = new_node(VARIABLE, L"банан", pos);
    push_node(main, new_node(KEYWORD, L"Выдра", pos));
    wprintf(L"%ls\n", main->value);
    wprintf(L"%ls\n", main->next->value);

    delete_tk_list(main);
    free(file);
    system("pause");
    return 0;
}