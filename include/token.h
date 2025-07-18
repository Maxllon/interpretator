#ifndef TOKEN_H
#define TOKEN_H

#include<my_link_list.h>
#include<defines.h>
#include<my_string.h>

typedef enum symb_type
{
    DIGIT,
    SEP,
    OP,
    LETTER,
    WHITE
}symb_type;

typedef enum token_type
{
    KEY_START,
    KEY_END,

    KEY_VARIABLE,
    KEY_NUMBER,
    KEY_STRING,
    KEY_KEYWORD,
    KEY_SEPARATOR,
    KEY_BOOL,
    KEY_INSTRYCTION,
    KEY_OP
}token_type;

typedef enum op_type
{
    KEY_PLUS,
    KEY_MINUS,
    KEY_NOT,
    KEY_AND,
    KEY_OR,
    KEY_MULT,
    KEY_DIV,
    KEY_FLOOR_DIV,
    KEY_POW,
    KEY_EQUAL,
    KEY_NOT_EQUAL,
    KEY_GREATER,
    KEY_LESS,
    KEY_GREATER_EQUAL,
    KEY_LESS_EQUAL,
    KEY_MOD,
    KEY_ASSIGN,
    KEY_AND_BIT,
    KEY_OR_BIT,
    KEY_XOR,
    KEY_XOR_BIT,
    KEY_NOT_BIT
}op_type;

typedef enum sep_type
{   
    KEY_DOT,
    KEY_COMMA,
    KEY_COLON,
    KEY_SEMICOLON,
    KEY_SQUARE_OPEN,
    KEY_SQUARE_CLOSE,
    KEY_PARENTHESIS_OPEN,
    KEY_PARENTHESIS_CLOSE,
}sep_type;

typedef enum key_type
{
    KEY_WHILE,
    KEY_FOREACH,
    KEY_IN,
    KEY_IF,
    KEY_ELSE,
    KEY_FUNC,
    KEY_CLASS,
    KEY_START_BLOCK,
    KEY_END_BLOCK,
    KEY_RETURN,
}key_type;

typedef enum instryct_type
{
    KEY_CONTINUE,
    KEY_BREAK
}instryction_type;

typedef enum bool_type
{
    KEY_TRUE,
    KEY_FALSE
}bool_type;

typedef struct symb_token
{
    char32_t c;
    symb_type type;
    pos _pos;
}symb_token;

typedef struct token
{
    token_type type;
    int subtype;
    string* value;
    pos _pos;
}token;

void
out_tk_list(link_node* root);
link_node*
tokenize(string* file);
#endif