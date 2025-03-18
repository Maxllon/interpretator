#include"defines.h"
#include<arena.h>

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");

    Arena* arena = arena_init();
    char* a = arena_alloc(arena, 10);
    strcpy(a, "aboba");
    a=arena_realloc(arena,a,11);
    printf("%ld\n", get_block_size(a));
    printf("%s", a);
    arena_destroy(arena);
    return 0;
}