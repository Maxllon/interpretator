#ifndef DEFINE_H
#define DEFINE_H

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<math.h>
#include<stddef.h>
#include<uchar.h>

#define True 1
#define False 0
#define bool_t uint8_t

#define LIST_DEFAULTT_LEN 16
#define OUT_SYMB_LIST 1
#define REGION_DEFAULT_CAPACITY 256

typedef struct pair pair;
struct pair
{
    void* first;
    void* second;
};

typedef struct pos pos;
struct pos
{
    size_t x,y;
};
#define def_pos(_pos, t1, t2) {_pos.x = t1; _pos.y = t2;}

#endif