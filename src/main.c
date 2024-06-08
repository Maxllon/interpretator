#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#include "read_file.h"
#include "lexer.h"
#include "parcer.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    wchar* file = read_file("example");
    if(out_file) wprintf(L"%ls\n\n\n\n",file);

    tk_node* main = lexing(file);

    struct Expretion* expr = parce(main);

    delete_tk_list(main);
    bm_free(file);
    delete_expr(expr);
    system("pause");
    return 0;
}