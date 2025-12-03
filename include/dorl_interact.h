#ifndef DORL_INTERACT_H
#define DORL_INTERACT_H

#include<defines.h>

typedef enum error_type
{
    ALL_GOOD,
    BAD_ALLOC,
    CANNOT_READ_FILE,
    UNKNOWN_SEP,
    WRONG_NUM,
    INCORRECT_STRING,
    UNKNOWN_OP,
    STR_WITHOUT_END,
    INCORRECT_LEFT,
    INCORRECT_INDEX,
    INCORRECT_ITER,
    DIV_ZERO,
    MISS_NUM,
    MISS_BOOL,
    UNKNOWN_UN,
    UNKNOWN_TOKEN,
    SYNTAX_ERROR,
    INCORRECT_NAME_OBJECT,
    OUT_OF_RANGE,
    INCORRECT_ARGS

}error_type;

typedef struct error error;
struct error
{
    error_type type;
    pos pos_start;
};
#endif