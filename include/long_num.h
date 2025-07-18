#ifndef LONG_NUM_H
#define LONG_NUM_H

#include<my_list.h>
#include<defines.h>

#define MAX_LONG_LEN 1000
#define BASE_BITS 9
#define BASE ((uint64_t)1e9)

typedef struct big_num big_num;
struct big_num
{
    vector* digits;
    int is_negative;
};

big_num*
big_num_init();

big_num*
big_num_from_str(char32_t* str);

char32_t* 
str_from_big_num(big_num* b_num);

big_num* 
big_num_from_szt(size_t num);

size_t 
szt_from_big_num(big_num* a);

big_num*
big_num_from_szt(size_t val);

big_num* 
sum_big(big_num* a, big_num* b);

big_num* 
sub_big(big_num* a, big_num* b);

big_num* 
mult_big(big_num* a, big_num* b);

big_num* 
div_big(big_num* a, big_num* b);

big_num* 
div_int_big(big_num* a, big_num* b);

big_num* 
mod_big(big_num* a, big_num* b);

big_num* 
pow_big(big_num* a, big_num* b);

big_num* 
sqr_big(big_num* a, big_num* N);

int 
compare_big(big_num* a, big_num* b);
int 
compare_big_normal(big_num* a, big_num* b);

#endif