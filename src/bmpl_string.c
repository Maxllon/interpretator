#include"bmpl_string.h"

static const size_t SIMPLES[] = 
{
    10007, 10009, 10031, 10037, 10039, 10061, 10067, 10069, 10079, 10091,
    10093, 10099, 10103, 10111, 10133, 10139, 10141, 10151, 10159, 10163,
    10169, 10177, 10181, 10183, 10187, 10189, 10193, 10199, 10211, 10223,
    10229, 1023
};

size_t get_hash(wchar* string, size_t k)
{
    size_t h = 0;
    size_t m = 1;
    for(;*string != L'\0';string++)
    {
        size_t x = (size_t)(*string - L'a' + 1);
        h = (h + m * x);
        m = (m * k);
    }
    return h;
}

bmpl_string* bmpl_string_from_str(wchar* string, Arena* arena)
{
    bmpl_string* res = arena_alloc(arena, sizeof(bmpl_string));
    res->size = wcslen(string);
    res->string = arena_alloc(arena, sizeof(wchar)*res->size + 2);
    wcscpy(res->string, string);
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        res->hashes[i] = get_hash(string, SIMPLES[HASH_COUNT]);
    }
    return res;
}
bmpl_string* bmpl_string_copy(bmpl_string* string, Arena* arena)
{
    bmpl_string* res = arena_alloc(arena, sizeof(bmpl_string));
    res->size = string->size;
    res->string = arena_alloc(arena, sizeof(wchar)*res->size + 2);
    wcscpy(res->string, string->string);
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        res->hashes[i] = string->hashes[i];
    }
    return res;
}
bmpl_string* bmpl_string_cancat(bmpl_string* a, bmpl_string* b, Arena* arena)
{
    bmpl_string* res = arena_alloc(arena, sizeof(bmpl_string));
    res->string = arena_alloc(arena, sizeof(wchar)*(a->size+b->size) + 2);
    res->size = a->size + b->size;
    wcscpy(res->string, a->string);
    wcscat(res->string, b->string);
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        res->hashes[i] = get_hash(res->string, SIMPLES[HASH_COUNT]);
    }
    return res;
}
bmpl_string* bmpl_string_mult(bmpl_string* a, big_num* b, Arena* arena)
{
    bmpl_string* res = arena_alloc(arena, sizeof(bmpl_string));
    size_t cnt =ull_from_big_num(b);
    res->size = a->size*cnt;
    res->string = arena_alloc(arena, sizeof(wchar)*res->size + 2);
    wcscpy(res->string, a->string);
    for(size_t i = 1; i < cnt; ++i)
    {
        wcscat(res->string, a->string);
    }
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        res->hashes[i] = get_hash(res->string, SIMPLES[HASH_COUNT]);
    }
    return res;
}
bmpl_string* slice(bmpl_string* string, big_num* i0, big_num* i1, Arena* arena)
{
    bmpl_string* res = arena_alloc(arena, sizeof(bmpl_string));
    size_t start =ull_from_big_num(i0);
    size_t end =ull_from_big_num(i1);
    res->string = arena_alloc(arena, sizeof(wchar)*(end-start) + 2);
    res->size = end-start;
    wchar* temp = res->string;
    for(size_t i=start; i<end;++i,temp++)
    {
        *temp = string->string[i];
    }
    *temp = L'\0';
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        res->hashes[i] = get_hash(res->string, SIMPLES[HASH_COUNT]);
    }
    return res;
}
int bmpl_string_equal(bmpl_string* a, bmpl_string* b)
{
    if(a->size != b->size) return 0;
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        if(a->hashes[i] != b->hashes[i]) return 0;
    }
    return 1;
}