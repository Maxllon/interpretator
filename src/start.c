#include<start.h>
#include<lexer.h>

int start(char* file,uint is_out_file,  uint is_out_tklist, uint is_out_tree)
{
    node_t* tk_list = lexing(file, is_out_file, is_out_tklist);
    if(tk_list == NULL)
    {
        printf("Error: token list is NULL\n");
        return 1;
    }

    del_node(tk_list);
    return 0;
}