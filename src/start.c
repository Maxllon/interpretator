#include<start.h>
#include<lexer.h>

int start(char* file,uint is_out_file,  uint is_out_tklist, uint is_out_tree)
{
    if (lexing(file, is_out_file, is_out_tklist)) return 1;

    return 0;
}