#include"long_num.h"

big_num* big_num_init(Arena* arena)
{
    big_num* b_num = arena_alloc(arena, sizeof(big_num));

    b_num->digits = bm_vector_create(arena);
    ull* zero = arena_alloc(arena, sizeof(ull));
    *zero = 0;
    bm_vector_push(b_num->digits, zero);
    zero = arena_alloc(arena, sizeof(ull));
    *zero = 0;
    bm_vector_push(b_num->digits, zero);


    b_num->is_negative = 0;

    return b_num;
}

//возвращает NULL если создать число из этой строки невозможно
big_num* big_num_from_str(const wchar* str, Arena* arena)
{
    if(str == NULL || arena == NULL) return NULL;
    big_num* b_num = big_num_init(arena);
    size_t len = wcslen(str);
    if(len==0) return NULL;
    if(str[0] == L'-')
    {
        str++;
        b_num->is_negative = 1;
        len--;
    }
 
    size_t dot_pos = 0;
    for(;str[dot_pos]!=L'.' && str[dot_pos]!=L'\0';++dot_pos){}

    ull* res = arena_alloc(arena, sizeof(ull));
    *res = 0;
    size_t cnt = BASE_BITS;
    if(dot_pos!=len)
    {
        size_t start;
        for(start = dot_pos+1; str[start]!=L'\0' && cnt!=0 && str[start] == L'0';++start,--cnt){}
        for(size_t i=start; str[i]!=L'\0' && cnt!=0;++i, cnt--)
        {
            *res*=10;
            *res+=str[i]-L'0';
        }
        for(size_t i=0;i<cnt;++i)
        {
            *res*=10;
        }
    }
    bm_vector_change(b_num->digits, 0, res);
    for(ll i=dot_pos-1, cnt=1;i!=-1;cnt++)
    {
        res = arena_alloc(arena, sizeof(ull));
        *res = 0;
        ull mult = 1;
        for(ull k=0;i!=-1 && k<BASE_BITS;--i,++k)
        {     
            *res += (str[i]-L'0')*mult;
            mult*=10;
        }
        bm_vector_change(b_num->digits, cnt, res);
    }
    return b_num;
}

big_num* big_num_from_ull(size_t num, Arena* arena)
{
    wchar* buff = arena_alloc(arena, sizeof(wchar) * 25);
    swprintf(buff, 25, L"%llu", num);
    return big_num_from_str(buff, arena);
}

static wchar_t* wcsrev_ptr(wchar_t* str) 
{
    if (!str) return NULL;
    
    wchar_t* start = str;
    wchar_t* end = str + wcslen(str) - 1;
    
    while (start < end) 
    {
        wchar_t temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    return str;
}

static wchar* byte_to_string(ull num, Arena* arena)
{
    wchar* res = arena_alloc(arena, sizeof(wchar) * BASE_BITS + 2);
    res[BASE_BITS] = L'\0';
    wchar* temp = res;
    int cnt = BASE_BITS;
    while(num!=0)
    {
        *temp = L'0' + num%10;
        temp++;
        num/=10;
        cnt--;
    }
    if(cnt==BASE_BITS) cnt--;
    for(;cnt!=-1;cnt--)
    {
        *temp = L'0';
        temp++;
    }
    return res;
}

wchar* str_from_big_num(big_num* b_num, Arena* arena)
{
    size_t len = 0;
    for(;bm_vector_at(b_num->digits, len) != NULL; ++len);
    wchar* res = arena_alloc(arena, sizeof(wchar) * len * BASE_BITS + 4);
    wchar* temp = res;
    wchar* byte;
    if(*(ull*)bm_vector_at(b_num->digits, 0) != 0)
    {
        byte = byte_to_string(*(ull*)bm_vector_at(b_num->digits, 0), arena);
        size_t cnt;
        for(cnt=0;*byte==L'0';byte++, cnt++);
        for(size_t i=cnt;i<BASE_BITS;++i,byte++,temp++)
        {
            *temp = *byte;
        }
        *temp = L'.';
        temp++;
    }
    size_t i;
    for(i = 1;(ull*)bm_vector_at(b_num->digits, i+1) != NULL; ++i)
    {
        byte = byte_to_string(*(ull*)bm_vector_at(b_num->digits, i), arena);
        for(size_t i=0;i<BASE_BITS;++i,byte++,temp++)
        {
            *temp = *byte;
        }
    }
    byte = byte_to_string(*(ull*)bm_vector_at(b_num->digits, i), arena);
    size_t cnt = 0;
    for(size_t i=BASE_BITS-1;i!=0 && byte[i] == L'0';--i,cnt++){}
    for(size_t i=0;i<BASE_BITS-cnt;++i,byte++,temp++)
    {
        *temp = *byte;
    }
    if(b_num->is_negative)
    {
        *temp = L'-';
    }
    temp++;
    *temp = L'\0';
    return wcsrev_ptr(res);
}

big_num* sum_big(big_num* a, big_num* b, Arena* arena)
{
    big_num* res = big_num_init(arena);
    if(a->is_negative && b->is_negative) res->is_negative = 1;
    else if(b->is_negative)
    {
        b->is_negative = 0;
        return sub_big(a, b, arena);
    }
    else if(a->is_negative) 
    {
        a->is_negative = 0;
        return sub_big(b, a, arena);
    }
    ull carry = 0;
    ull sum = 0;
    for(size_t i=0;(ull*)bm_vector_at(a->digits, i) != NULL || (ull*)bm_vector_at(b->digits, i) != NULL || carry;++i)
    {
        sum = carry;
        ull* ptr = arena_alloc(arena, sizeof(ull));
        if((ull*)bm_vector_at(a->digits, i) != NULL) sum+=*(ull*)bm_vector_at(a->digits, i);
        if((ull*)bm_vector_at(b->digits, i) != NULL) sum+=*(ull*)bm_vector_at(b->digits, i);
        *ptr = sum%BASE;
        carry = sum/BASE;
        bm_vector_change(res->digits, i, ptr);
    }
    return res;
}

big_num* sub_big(big_num* a, big_num* b, Arena* arena)
{
    big_num* res = big_num_init(arena);
    if(!a->is_negative && b->is_negative)
    {
        b->is_negative = 0;
        return sum_big(a, b, arena);
    }
    if(a->is_negative && !b->is_negative)
    {
        b->is_negative = 1;
        return sum_big(a, b, arena);
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

    ull borrow = 0;
    for (size_t i = 0; (ull*)bm_vector_at(a->digits, i)!=NULL || borrow; ++i) 
    {
        ull a_digit = ((ull*)bm_vector_at(a->digits, i)!=NULL) ? *(ull*)bm_vector_at(a->digits, i) : 0;
        ull b_digit = ((ull*)bm_vector_at(b->digits, i)!=NULL) ? *(ull*)bm_vector_at(b->digits, i) : 0;
        borrow = 0;

        if (a_digit<b_digit) 
        {
            a_digit+= BASE;
            borrow = 1;
        }

        ull* ptr = arena_alloc(arena, sizeof(ull));
        *ptr = a_digit-b_digit;
        bm_vector_change(res->digits, i, ptr);
    }
    size_t i;
    for(i = 1;(ull*)bm_vector_at(res->digits, i+1) != NULL; ++i);
    for(;*(ull*)bm_vector_at(res->digits, i) == 0 && i > 1; --i)
    {
        bm_vector_change(res->digits, i, NULL);
    }
    return res;

}

big_num* mult_big(big_num* a, big_num* b, Arena* arena)
{
    big_num* res = big_num_init(arena);
    if(a->is_negative != b->is_negative) res->is_negative = 1;
    ull carry = 0;
    for (size_t i = 0; (ull*)bm_vector_at(a->digits, i)!=NULL; ++i) 
    {
        for (size_t j = 0; (ull*)bm_vector_at(b->digits, j)!=NULL || carry; ++j) 
        {
            ull a_digit = *(ull*)bm_vector_at(a->digits, i);
            ull b_digit = ((ull*)bm_vector_at(b->digits, j)!=NULL) ? *(ull*)bm_vector_at(b->digits, j) : 0;
            if(i+j==0)
            {
                ull sum = (*(ull*)bm_vector_at(b->digits, j))*(*(ull*)bm_vector_at(a->digits, i));
                carry = sum / BASE;
            }
            else
            {
                ull res_digit = ((ull*)bm_vector_at(res->digits, i+j-1)!=NULL) ? *(ull*)bm_vector_at(res->digits, i + j-1) : 0;
            
                ull sum = carry + res_digit + a_digit * b_digit;
                ull* ptr = arena_alloc(arena, sizeof(ull));
                *ptr = sum % BASE;
                carry = sum / BASE;
                bm_vector_change(res->digits, i + j-1, ptr);
            }
        }
    }
    return res;
}

big_num* div_big(big_num* a, big_num* b, Arena* arena)
{
    big_num* res = big_num_init(arena);
    big_num* zero = big_num_from_str(L"0", arena);
    if(!compare_big(b, zero)) return NULL;
    if(!compare_big(a, zero)) return zero;
    if(a->is_negative != b->is_negative) res->is_negative = 1;

    size_t cnt_digits = 0;

    for(; compare_big(mult_big(res, b, arena), a) == -1; cnt_digits++)
    {
        ull* ptr = arena_alloc(arena, sizeof(ull));
        *ptr = 0;
        bm_vector_change(res->digits, cnt_digits, ptr);
        ptr = arena_alloc(arena, sizeof(ull));
        *ptr = 1;
        bm_vector_change(res->digits, cnt_digits+1, ptr);
    }
    if(cnt_digits>1) bm_vector_change(res->digits, cnt_digits, NULL);
    else
    {
        ull* ptr = arena_alloc(arena, sizeof(ull));
        *ptr = 0;
        bm_vector_change(res->digits, cnt_digits, ptr);
    }
    for(ll i = cnt_digits-1; i!=-1;--i)
    {
        ull* ptr = arena_alloc(arena, sizeof(ull));
        bm_vector_change(res->digits, i, ptr);
        ull l = 0;
        ull r = BASE;
        while(r-l > 1)
        {
            ull m = (l+r)/2;
            *ptr = m;
            if(compare_big(mult_big(res, b, arena), a) != 1)
            {
                l = m;
            }
            else r = m;
        }
        *ptr = l;
    }
    return res;
}

big_num* div_int_big(big_num* a, big_num* b, Arena* arena)
{
    big_num* res = div_big(a, b, arena);
    if(res==NULL) return res;
    ull* ptr = arena_alloc(arena, sizeof(ull));
    *ptr = 0;
    bm_vector_change(res->digits, 0, ptr);
    return res;
}

big_num* mod_big(big_num* a, big_num* b, Arena* arena)
{
    big_num* res = div_int_big(a, b, arena);
    if(res==NULL) return NULL;
    return sub_big(a, mult_big(b, res, arena), arena);
}

int compare_big(big_num* a, big_num* b)
{
    int res = 0;
    for(size_t i=0;(ull*)bm_vector_at(a->digits, i) != NULL || (ull*)bm_vector_at(b->digits, i) != NULL;++i)
    {
        if((ull*)bm_vector_at(a->digits, i) == NULL) return -1;
        if((ull*)bm_vector_at(b->digits, i) == NULL) return 1;
        ull a_digit = *(ull*)bm_vector_at(a->digits, i);
        ull b_digit = *(ull*)bm_vector_at(b->digits, i);
        if(a_digit > b_digit) res = 1;
        if(b_digit > a_digit) res = -1;
    }
    return res;
}
int compare_big_normal(big_num* a, big_num* b)
{
    if(a->is_negative != b->is_negative) return a->is_negative ? -1 : 1;
    int res = compare_big(a, b);
    if(a->is_negative) res*=-1;
    return res;
}
