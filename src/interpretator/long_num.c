#include<long_num.h>
#include<my_string.h>
#include<globals.h>

big_num* 
big_num_init()
{
    big_num* b_num = arena_alloc(sizeof(big_num));
    if(b_num == NULL) return NULL;
    if(!(b_num->digits = list_init())) return NULL;
    
    uint64_t* zero = arena_alloc(sizeof(uint64_t));
    if(zero == NULL) return NULL;
    *zero = 0;
    if(list_push(b_num->digits, zero) != ALL_GOOD) return NULL;
    zero = arena_alloc(sizeof(uint64_t));
    if(zero == NULL) return NULL;
    *zero = 0;
    if(list_push(b_num->digits, zero) != ALL_GOOD) return NULL;
    b_num->is_negative = 0;
    return b_num;
}



big_num* 
big_num_from_str(char32_t* buff)
{
    if(buff == NULL) return NULL;
    big_num* b_num = big_num_init();
    if(b_num == NULL) return NULL;
    size_t len = utf32_len(buff);
    if(len==0) return NULL;
    if(buff[0] == U'-')
    {
        buff++;
        b_num->is_negative = 1;
        len--;
    }
 
    size_t dot_pos = 0;
    for(;buff[dot_pos]!=U'.' && buff[dot_pos]!=L'\0';++dot_pos){}

    uint64_t* res = arena_alloc(sizeof(uint64_t));
    if(res == NULL) return NULL;
    *res = 0;
    size_t cnt = BASE_BITS;
    if(dot_pos!=len)
    {
        size_t start;
        for(start = dot_pos+1; buff[start]!=U'\0' && cnt!=0 && buff[start] == U'0';++start,--cnt){}
        for(size_t i=start; buff[i]!=U'\0' && cnt!=0;++i, cnt--)
        {
            *res*=10;
            *res+=buff[i]-U'0';
        }
        for(size_t i=0;i<cnt;++i)
        {
            *res*=10;
        }
    }
    if(list_change(b_num->digits, res, 0) != ALL_GOOD) return NULL;
    for(int64_t i=dot_pos-1, cnt=1;i!=-1;cnt++)
    {
        if(!(res = arena_alloc(sizeof(uint64_t)))) return NULL;
        *res = 0;
        size_t mult = 1;
        for(size_t k=0;i!=-1 && k<BASE_BITS;--i,++k)
        {     
            *res += (buff[i]-U'0')*mult;
            mult*=10;
        }
        if(list_change(b_num->digits, res, cnt) != ALL_GOOD) return NULL;
    }
    return b_num;
}

static char32_t* 
utf32rev_ptr(char32_t* str) 
{
    if (!str) return NULL;
    
    char32_t* start = str;
    char32_t* end = str + utf32_len(str) - 1;
    
    while (start < end) 
    {
        char32_t temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    return str;
}

static char32_t* 
byte_to_string(uint64_t num)
{
    char32_t* res = arena_alloc(sizeof(char32_t) * (BASE_BITS+1));
    if(!res) return NULL;
    res[BASE_BITS] = U'\0';
    char32_t* temp = res;
    int cnt = BASE_BITS;
    while(num!=0)
    {
        *temp = U'0' + num%10;
        temp++;
        num/=10;
        cnt--;
    }
    if(cnt==BASE_BITS) cnt--;
    for(;cnt!=-1;cnt--)
    {
        *temp = U'0';
        temp++;
    }
    return res;
}

char32_t* 
str_from_big_num(big_num* b_num)
{
    size_t len = 0;
    for(;list_at(b_num->digits, len) != NULL; ++len);
    char32_t* res = arena_alloc(sizeof(char32_t) * len * BASE_BITS + 8);
    if(!res) return NULL;
    char32_t* temp = res;
    char32_t* byte;
    if(*(uint64_t*)list_at(b_num->digits, 0) != 0)
    {
        byte = byte_to_string(*(uint64_t*)list_at(b_num->digits, 0));
        if(!byte) return NULL;
        size_t cnt;
        for(cnt=0;*byte==U'0';byte++, cnt++);
        for(size_t i=cnt;i<BASE_BITS;++i,byte++,temp++)
        {
            *temp = *byte;
        }
        *temp = U'.';
        temp++;
    }
    size_t i;
    for(i = 1;(uint64_t*)list_at(b_num->digits, i+1) != NULL; ++i)
    {
        byte = byte_to_string(*(uint64_t*)list_at(b_num->digits, i));
        if(!byte) return NULL;
        for(size_t i=0;i<BASE_BITS;++i,byte++,temp++)
        {
            *temp = *byte;
        }
    }
    byte = byte_to_string(*(uint64_t*)list_at(b_num->digits, i));
    if(!byte) return NULL;
    size_t cnt = 0;
    for(size_t i=BASE_BITS-1;i!=0 && byte[i] == U'0';--i,cnt++){}
    for(size_t i=0;i<BASE_BITS-cnt;++i,byte++,temp++)
    {
        *temp = *byte;
    }
    if(b_num->is_negative)
    {
        *temp = U'-';
    }
    temp++;
    *temp = U'\0';
    return utf32rev_ptr(res);
}

size_t 
szt_from_big_num(big_num* a)
{
    char32_t* str = str_from_big_num(a);
    size_t res = 0;
    while(*str != U'\0')
    {
        if(*str == U'-') str++;
        res *= 10;
        res += *str - U'0';
        str++;
    }
    return res;
}

big_num*
big_num_from_szt(size_t val)
{
    string* str = str_init(U"");
    if(str == NULL) return NULL;
    if(val == 0)
    {
        str_push_c(str, U'0');
    }
    while(val > 0)
    {
        str_push_c(str, U'0' + val%10);
        val/=10;
    }
    str_reverse(str);
    return big_num_from_str(str->_str);
}

int 
compare_big(big_num* a, big_num* b)
{
    int res = 0;
    for(size_t i=0;(uint64_t*)list_at(a->digits, i) != NULL || (uint64_t*)list_at(b->digits, i) != NULL;++i)
    {
        if((uint64_t*)list_at(a->digits, i) == NULL) return -1;
        if((uint64_t*)list_at(b->digits, i) == NULL) return 1;
        uint64_t a_digit = *(uint64_t*)list_at(a->digits, i);
        uint64_t b_digit = *(uint64_t*)list_at(b->digits, i);
        if(a_digit > b_digit) res = 1;
        if(b_digit > a_digit) res = -1;
    }
    return res;
}
int
compare_big_normal(big_num* a, big_num* b)
{
    if(a->is_negative != b->is_negative) return a->is_negative ? -1 : 1;
    int res = compare_big(a, b);
    if(a->is_negative) res*=-1;
    return res;
}

big_num*
sum_big(big_num* a, big_num* b)
{
    big_num* res = big_num_init();
    if(!res) return NULL;
    if(a->is_negative && b->is_negative) res->is_negative = 1;
    else if(b->is_negative)
    {
        b->is_negative = 0;
        return sub_big(a, b);
    }
    else if(a->is_negative) 
    {
        a->is_negative = 0;
        return sub_big(b, a);
    }
    uint64_t carry = 0;
    uint64_t sum = 0;
    for(size_t i=0;(uint64_t*)list_at(a->digits, i) != NULL || (uint64_t*)list_at(b->digits, i) != NULL || carry;++i)
    {
        sum = carry;
        uint64_t* ptr = arena_alloc(sizeof(uint64_t));
        if(!ptr) return NULL;
        if((uint64_t*)list_at(a->digits, i) != NULL) sum+=*(uint64_t*)list_at(a->digits, i);
        if((uint64_t*)list_at(b->digits, i) != NULL) sum+=*(uint64_t*)list_at(b->digits, i);
        *ptr = sum%BASE;
        carry = sum/BASE;
        if(list_change(res->digits, ptr, i) != ALL_GOOD) return NULL;
    }
    return res;
}

big_num* 
sub_big(big_num* a, big_num* b)
{
    big_num* res = big_num_init();
    if(!a->is_negative && b->is_negative)
    {
        b->is_negative = 0;
        return sum_big(a, b);
    }
    if(a->is_negative && !b->is_negative)
    {
        b->is_negative = 1;
        return sum_big(a, b);
    }
    if(a->is_negative && b->is_negative)
    {
        b->is_negative = 0;
        a->is_negative = 0;
        big_num* temp = a;
        a = b;
        b = temp;
    }

    int cmp = compare_big(a, b);
    if(cmp == -1)
    {
        res->is_negative = 1;
        big_num* temp = a;
        a = b;
        b = temp;
    }

    uint64_t borrow = 0;
    for (size_t i = 0; (uint64_t*)list_at(a->digits, i)!=NULL || borrow; ++i) 
    {
        uint64_t a_digit = ((uint64_t*)list_at(a->digits, i)!=NULL) ? *(uint64_t*)list_at(a->digits, i) : 0;
        uint64_t b_digit = ((uint64_t*)list_at(b->digits, i)!=NULL) ? *(uint64_t*)list_at(b->digits, i) : 0;
        borrow = 0;

        if (a_digit<b_digit) 
        {
            a_digit+= BASE;
            borrow = 1;
        }

        uint64_t* ptr = arena_alloc(sizeof(uint64_t));
        if(!ptr) return NULL;
        *ptr = a_digit-b_digit;
        if(list_change(res->digits, ptr, i) != ALL_GOOD) return NULL;
    }
    size_t i;
    for(i = 1;(uint64_t*)list_at(res->digits, i+1) != NULL; ++i);
    for(;*(uint64_t*)list_at(res->digits, i) == 0 && i > 1; --i)
    {
        if(list_change(res->digits, NULL, i) != ALL_GOOD) return NULL;
    }
    return res;

}

big_num*
mult_big(big_num* a, big_num* b)
{
    big_num* res = big_num_init();
    if(!res) return NULL;
    if(a->is_negative != b->is_negative) res->is_negative = 1;
    uint64_t carry = 0;
    for (size_t i = 0; (uint64_t*)list_at(a->digits, i)!=NULL; ++i) 
    {
        for (size_t j = 0; (uint64_t*)list_at(b->digits, j)!=NULL || carry; ++j) 
        {
            uint64_t a_digit = *(uint64_t*)list_at(a->digits, i);
            uint64_t b_digit = ((uint64_t*)list_at(b->digits, j)!=NULL) ? *(uint64_t*)list_at(b->digits, j) : 0;
            if(i+j==0)
            {
                uint64_t sum = (*(uint64_t*)list_at(b->digits, j))*(*(uint64_t*)list_at(a->digits, i));
                carry = sum / BASE;
            }
            else
            {
                uint64_t res_digit = ((uint64_t*)list_at(res->digits, i+j-1)!=NULL) ? *(uint64_t*)list_at(res->digits, i + j-1) : 0;
            
                uint64_t sum = carry + res_digit + a_digit * b_digit;
                uint64_t* ptr = arena_alloc(sizeof(uint64_t));
                if(!ptr) return NULL;
                *ptr = sum % BASE;
                carry = sum / BASE;
                if(list_change(res->digits, ptr, i + j-1) != ALL_GOOD) return NULL;
            }
        }
    }
    return res;
}

big_num* 
div_big(big_num* a, big_num* b)
{
    big_num* res = big_num_init();
    if(!res) return NULL;
    big_num* zero = big_num_from_str(U"0");
    if(!zero) return NULL;
    if(!compare_big(b, zero)) return NULL;
    if(!compare_big(a, zero)) return zero;
    if(a->is_negative != b->is_negative) res->is_negative = 1;
    if(!compare_big(a, b))
    {
        int ise = res->is_negative;
        res = big_num_from_str(U"1");
        res->is_negative = ise;
        return res;
    }

    size_t cnt_digits = 0;
    for(; compare_big(mult_big(res, b), a) == -1; cnt_digits++)
    {
        uint64_t* ptr = arena_alloc(sizeof(uint64_t));
        if(!ptr) return NULL;
        *ptr = 0;
        if(list_change(res->digits, ptr, cnt_digits) != ALL_GOOD) return NULL;
        ptr = arena_alloc(sizeof(uint64_t));
        *ptr = 1;
        if(list_change(res->digits, ptr, cnt_digits+1) != ALL_GOOD) return NULL;
    }
    if(cnt_digits>1)
    {
        if(list_change(res->digits, NULL, cnt_digits) != ALL_GOOD) return NULL;
    }
    else
    {
        uint64_t* ptr = arena_alloc(sizeof(uint64_t));
        if(!ptr) return NULL;
        *ptr = 0;
        if(list_change(res->digits, ptr, cnt_digits) != ALL_GOOD) return NULL;
    }
    for(int64_t i = cnt_digits-1; i!=-1;--i)
    {
        uint64_t* ptr = arena_alloc(sizeof(uint64_t));
        if(!ptr) return NULL;
        if(list_change(res->digits, ptr, i) != ALL_GOOD) return NULL;
        uint64_t l = 0;
        uint64_t r = BASE;
        while(r-l > 1)
        {
            uint64_t m = (l+r)/2;
            *ptr = m;
            if(compare_big(mult_big(res, b), a) != 1)
            {
                l = m;
            }
            else r = m;
        }
        *ptr = l;
    }
    return res;
}

big_num*
div_int_big(big_num* a, big_num* b)
{
    big_num* res = div_big(a, b);
    if(res==NULL) return res;
    uint64_t* ptr = arena_alloc(sizeof(uint64_t));
    if(!ptr) return NULL;
    *ptr = 0;
    if(list_change(res->digits, ptr, 0) != ALL_GOOD) return NULL;
    return res;
}

big_num*
 mod_big(big_num* a, big_num* b)
{
    big_num* res = div_int_big(a, b);
    if(res==NULL) return NULL;
    return sub_big(a, mult_big(b, res));
}

//б должно быть целым
big_num*
pow_big(big_num* a, big_num* b)
{
    big_num* zero = big_num_from_str(U"0");
    big_num* one = big_num_from_str(U"1");
    big_num* two = big_num_from_str(U"2");
    if(!zero || !one || !two) return NULL;
    if(!compare_big(b, one)) return a;
    if(!compare_big(b, zero)) return one;
    big_num* res = pow_big(a, div_int_big(b, big_num_from_str(U"2")));
    if(!res) return NULL;
    res = mult_big(res, res);
    if(compare_big(zero, mod_big(b, two))) res = mult_big(res, a);
    return res;
}

#define precision 20
big_num*
sqr_big(big_num* a, big_num* N)
{
    big_num* one = big_num_from_str(U"1");
    if(!one) return NULL;
    big_num* N_min = sub_big(N, one);
    if(!N_min) return NULL;
    big_num* x = big_num_from_str(U"0.5");
    if(!x) return NULL;
    for(int i=0;i<precision;i++)
    {
        x = div_big(sum_big(mult_big(N_min, x), div_big(a, pow_big(x, N_min))), N);
        if(!x) return NULL;
    }
    return x;
}