#ifndef LEXER_H
#define LEXER_H

#include<includes.h>
#include<token_list.h>

node_t* lexing(char* file, uint is_out_file, uint is_out_tklist);

#endif