#ifndef MY_STRING_H
#define MY_STRING_H

#include<defines.h>
#define HASH_COUNT 20

typedef struct string string;

struct string
{
    char32_t* buffer;
    char32_t* _str;
    size_t len;
    size_t capacity;
    size_t indent;
    size_t hashes[HASH_COUNT];
};  
size_t
utf32_len(char32_t* buff);

string*
str_init(char32_t* buff);

bool_t
str_equal(string* str1, string* str2);

bool_t
str_equal_fast(string* str1, string* str2);

void
str_clear(string* str);

int
str_cancat(string* str, char32_t* right);

int
str_push_c(string* str, char32_t c);

int
str_cut(string* str, size_t start, size_t end);

void
str_reverse(string* str);

void
utf8_to_utf32(string* str, char* content);

void
str_out(string* str);


#endif