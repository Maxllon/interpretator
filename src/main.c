//Headers
#include<includes.h>
#include<read_file.h>

int main(int argc, char* argv[])
{
    wchar_t* file = read_file("example.txt");
    wprintf(L"%lc",file[0]);
    return 0;
}