#ifndef INCLUDES_H
#define INCLUDES_H

//includes
#include<stdlib.h>
#include<stdint.h>
#include<wchar.h>
#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<errno.h>

//constants
#define wchar wchar_t
#define EXPAN ".txt"
#define INDENT 4
typedef int Errno;


//functions
#define bm_wcscat(str1, str2) {str1 = realloc(str1, 2*(wcslen(str2) + wcslen(str1) + 1)); wcscat(str1, str2);}
#define error(message, pos) {printf("Error in pos { %d | %d }: %s\n", pos.x, pos.y, message); system("pause"); exit(1);}
#define emp_str(str) {str = malloc(sizeof(wchar) * 2);*str = sym;*(str+1) = L'\0';}
#define bm_free(ptr) {if(ptr != NULL) free(ptr); ptr = NULL;}

//outputs
#define out_symbols 1
#define out_tklist 1
#define out_file 1
#define out_tree 1

#endif