#ifndef DEFINES_H
#define DEFINES_H

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>

#define byte uint8_t
#define ARENA_MEM_BLOCK 1024

#define free(ptr){if(ptr != NULL) free(ptr); ptr = NULL;}

#endif