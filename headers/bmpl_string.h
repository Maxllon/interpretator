#ifndef BMPL_STRING_H
#define BMPL_STRING_H

#include"includes.h"
#include"arena.h"
#include"long_num.h"

#define HASH_COUNT 20

typedef struct bmpl_string bmpl_string;
struct bmpl_string
{
    wchar* string;
    size_t size;
    size_t hashes[HASH_COUNT];
};

size_t get_hash(wchar* string,  size_t k);
bmpl_string* bmpl_string_from_str(wchar* string, Arena* arena);
bmpl_string* bmpl_string_copy(bmpl_string* string, Arena* arena);
bmpl_string* bmpl_string_cancat(bmpl_string* a, bmpl_string* b, Arena* arena);
bmpl_string* bmpl_string_mult(bmpl_string* a, big_num* b, Arena* arena);
bmpl_string* slice(bmpl_string* string, big_num* i0, big_num* i1, Arena* arena);
int bmpl_string_equal(bmpl_string* a, bmpl_string* b);

#endif