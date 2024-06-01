#ifndef INCLUDES_H
#define INCLUDES_H

#include<stdlib.h>
#include<stdint.h>
#include<wchar.h>
#include<stdio.h>
#include<string.h>
#include<locale.h>

#define u8 uint8_t
#define u16 uint16_t
#define wchar wchar_t
#define expan ".txt"
#define fbuf 10000

#define bm_wcscat(str1, str2) str1 = realloc(str1, 2*wcslen(str2)); wcscat(str1, str2)

#endif