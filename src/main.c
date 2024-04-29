//Headers
#include<includes.h>
#include<read_file.h>
#include<start.h>
#include"lexer.c"

int main(int argc, char* argv[])
{
    char filename[100];
    char* file;
    uint is_out_tklist;
    uint is_out_tree;
    uint is_end = 0;
    uint is_out_file;

    while(!is_end)
    {
        printf("Define the interpretation configuration below:\n");

        printf("Enter the name of file: ");
        scanf("%s",&filename);

        printf("Display file (1|0): ");
        scanf("%d",&is_out_file);

        printf("Display token list (1|0): ");
        scanf("%d",&is_out_tklist);
        
        printf("Display AST (1|0): ");
        scanf("%d",&is_out_tree);

        file = read_file(filename);
        if(file != NULL)
        {
            if(start(file, is_out_file, is_out_tklist,is_out_tree)) printf("Something is went wrong...\n");

            free(file);
        }
        else printf("Something is went wrong...\n");
        
        printf("end? (1|0): ");
        scanf("%d",&is_end);
    }
    return 0;
}