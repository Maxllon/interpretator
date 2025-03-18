#include<defines.h>

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");
    wchar_t a[] = L"Привет мир!";
    printf("%lc", a[1]); 
    system("pause");
    return 0;
}