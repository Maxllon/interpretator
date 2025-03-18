#ifndef DEFINES_H
#define DEFINES_H

#include<string.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
typedef uint8_t byte;
typedef wchar_t wchar;

#define ARENA_MEM_BLOCK 1024

#define free(ptr){if(ptr != NULL){free(ptr); ptr = NULL;}}
#define get_block_size(ptr) ((ptr != NULL) ? *(size_t*)((byte*)ptr - sizeof(size_t)) : 0)

#endif