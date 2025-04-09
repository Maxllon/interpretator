#ifndef LONG_NUM_H
#define LONG_NUM_H

#include"includes.h"
#include"arena.h"
#include"bm_vector.h"

#define MAX_LONG_LEN 1000

typedef struct big_num big_num;
struct big_num
{
    bm_vector* digits;
    int is_negative;
    Arena* arena;
};

big_num* big_num_init(Arena* arena);

big_num* big_num_from_str(const wchar* str, Arena* arena);
wchar* str_from_big_num(big_num* b_num, Arena* arena);
big_num* big_num_from_ull(size_t num, Arena* arena);

big_num* sum_big(big_num* a, big_num* b, Arena* arena);
big_num* sub_big(big_num* a, big_num* b, Arena* arena);
big_num* mult_big(big_num* a, big_num* b, Arena* arena);
big_num* div_big(big_num* a, big_num* b, Arena* arena);
big_num* div_int_big(big_num* a, big_num* b, Arena* arena);
big_num* mod_big(big_num* a, big_num* b, Arena* arena);

int compare_big(big_num* a, big_num* b);
int compare_big_normal(big_num* a, big_num* b);

#endif