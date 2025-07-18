#include<defines.h>
#include<my_string.h>
#include<arena.h>
#include<dorl_interact.h>
#include<globals.h>

size_t
utf32_len(char32_t* buff)
{
    size_t res = 0;
    while(*buff != U'\0')
    {
        buff++;
        res++;
    }
    return res;
}

static const size_t SIMPLES[] = 
{
    10007, 10009, 10031, 10037, 10039, 10061, 10067, 10069, 10079, 10091,
    10093, 10099, 10103, 10111, 10133, 10139, 10141, 10151, 10159, 10163,
    10169, 10177, 10181, 10183, 10187, 10189, 10193, 10199, 10211, 10223,
    10229, 1023
};

static size_t
get_hash(char32_t* str, size_t k)
{
    size_t h = 0;
    size_t m = 1;
    for(;*str != U'\0';str++)
    {
        size_t x = (size_t)(*str - U'a' + 1);
        h = (h + m * x);
        m = (m * k);
    }
    return h;
}

string*
str_init(char32_t* buff)
{
    string* str = arena_alloc(sizeof(string));
    if(str == NULL)
    {
        return NULL;
    }
    str->len = utf32_len(buff);
    str->capacity = str->len + 1;
    str->buffer = arena_alloc((str->len + 1) * 4);
    str->_str = str->buffer;
    str->indent = 0;
    if(str->buffer == NULL)
    {
        return NULL;
    }
    memcpy(str->buffer, buff, (str->len + 1) * 4);
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        str->hashes[i] = get_hash(str->_str, SIMPLES[i]);
    }
    return str;
}

bool_t
str_equal(string* str1, string* str2)
{
    if(str1->len != str2->len) return False;
    for(size_t i=0;i<str1->len;++i)
    {
        if(str1->_str[i] != str2->_str[i]) return False;
    }
    return True;
}

bool_t
str_equal_fast(string* str1, string* str2)
{
    if(str1->len != str2->len) return False;
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        if(str1->hashes[i] != str2->hashes[i]) return False;
    }
    return True;
}

void
str_clear(string* str)
{
    str->capacity += str->len;
    str->len = 0;
    str->indent = 0;
    str->_str = str->buffer;
    str->buffer[0] = U'\0';
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        str->hashes[i] = get_hash(str->_str, SIMPLES[i]);
    }
}

int
str_cancat(string* str, char32_t* right)
{
    size_t r_len = utf32_len(right);
    size_t n_len = str->len + r_len;
    char32_t* temp;
    if(str->capacity < n_len + 1 + str->indent)
    {
        str->capacity = (str->capacity * 2) + r_len;

        temp = arena_realloc(str->buffer, str->capacity*4);
        if(temp == NULL)
        {
            return BAD_ALLOC;
        }
        str->buffer = temp;
        str->_str = str->buffer + str->indent;
    }
    memcpy(str->_str+ str->len, right, (r_len+1)*4);
    str->len = n_len;
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        str->hashes[i] = get_hash(str->_str, SIMPLES[i]);
    }
    return ALL_GOOD;
}

int
str_push_c(string* str, char32_t c)
{
    char32_t right[2];
    right[0] = c;
    right[1] = U'\0';
    size_t r_len = utf32_len(right);
    size_t n_len = str->len + r_len;
    char32_t* temp;
    if(str->capacity < n_len + 1 + str->indent)
    {
        str->capacity = (str->capacity * 2) + r_len;

        temp = arena_realloc(str->buffer, str->capacity*4);
        if(temp == NULL)
        {
            return BAD_ALLOC;
        }
        str->buffer = temp;
        str->_str = str->buffer + str->indent;
    }
    memcpy(str->_str+ str->len, right, (r_len+1)*4);
    str->len = n_len;
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        str->hashes[i] = get_hash(str->_str, SIMPLES[i]);
    }
    return ALL_GOOD;
}

int
str_cut(string* str, size_t start, size_t end)
{
    if(start >= str->len || end >= str->len || start > end)
    {
        return LAST_ERROR.type = OUT_OF_RANGE;
    }
    str->indent += start;
    str->_str += start;
    str->len = end-start+1;
    str->_str[str->len] = U'\0';
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        str->hashes[i] = get_hash(str->_str, SIMPLES[i]);
    }
    return ALL_GOOD;
}

void
str_reverse(string* str)
{
    char32_t c;
    for(size_t i = 0; i < str->len/2; ++i)
    {
        c = str->_str[i];
        str->_str[i] = str->_str[str->len-1-i];
        str->_str[str->len-1-i] = c;
    }
    for(size_t i=0;i<HASH_COUNT;++i)
    {
        str->hashes[i] = get_hash(str->_str, SIMPLES[i]);
    }
}

static void 
utf32_to_utf8(char32_t codepoint, char *utf8_buf) 
{
    if (codepoint <= 0x7F) 
    {
        utf8_buf[0] = (char)codepoint;
        utf8_buf[1] = '\0';
    } 
    else if (codepoint <= 0x7FF) 
    {
        utf8_buf[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
        utf8_buf[1] = 0x80 | (codepoint & 0x3F);
        utf8_buf[2] = '\0';
    } 
    else if (codepoint <= 0xFFFF) 
    {
        utf8_buf[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
        utf8_buf[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8_buf[2] = 0x80 | (codepoint & 0x3F);
        utf8_buf[3] = '\0';
    }
    else if (codepoint <= 0x10FFFF) 
    {
        utf8_buf[0] = 0xF0 | ((codepoint >> 18) & 0x07);
        utf8_buf[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        utf8_buf[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8_buf[3] = 0x80 | (codepoint & 0x3F);
        utf8_buf[4] = '\0';
    }
}

static char32_t
utf8_to_utf32_c(char* utf8, int* indent)
{
    char32_t res = 0;
    uint8_t first_byte = (uint8_t) utf8[0];
    if(first_byte <= 0x7F)
    {
        *indent = 1;
        res |= first_byte;
    }
    else if(first_byte <= 0xDF)
    {
        *indent = 2;
        res |= (first_byte & 0x1F) << 6;
        res |= (utf8[1] & 0x3F);
    }
    else if(first_byte <= 0xEF)
    {
        *indent = 3;
        res |= (first_byte & 0x0F) << 12;
        res |= (utf8[1] & 0x3F) << 6;
        res |= (utf8[2] & 0x3F);
    }
    else if(first_byte <= 0xF7)
    {
        *indent = 4;
        res |= (first_byte & 0x0F) << 18;
        res |= (utf8[1] & 0x3F) << 12;
        res |= (utf8[2] & 0x3F) << 6;
        res |= (utf8[3] & 0x3F);
    }
    return res;
}

void
utf8_to_utf32(string* str, char* content)
{
    int indent;
    while(*content != '\0')
    {
        str_push_c(str, utf8_to_utf32_c(content, &indent));
        content+=indent;
    }
}

void
str_out(string* str)
{
    char32_t* buff = str->_str;
    for (int i = 0; buff[i] != 0; i++) 
    {
        char utf8_buf[5];
        utf32_to_utf8(buff[i], utf8_buf);
        printf("%s", utf8_buf);
    }
}