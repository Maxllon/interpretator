#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#include "read_file.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    wchar* file = read_file("example");
    
    wchar* f = file;
    wprintf(L"%ls", f);

    free(file);
    system("pause");
    return 0;
}