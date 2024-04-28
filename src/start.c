#include<start.h>
#include<lexer.h>

void start(char* file,uint is_out_file,  uint is_out_tklist, uint is_out_tree)
{
    lexing(file, is_out_file, is_out_tklist);
}