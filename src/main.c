//C libs
#include<stdio.h>

//Headers
#include<read_file.h>

#define uint unsigned
int main(int argc, char* argv[])
{
    char* filename;
    uint is_out_tklist;
    uint is_out_tree;
    uint is_end = 0;

    while(!is_end)
    {
        printf("Define the interpretation configuration below:\n");
        printf("Enter the name of file: ");
        scanf("%s",&filename);
        printf("Display token list (1|0): ");
        scanf("%d",&is_out_tklist);
        printf("Display AST (1|0): ");
        scanf("%d",&is_out_tree);

        if(!start(filename,is_out_tklist,is_out_tree)) printf("Something is went wrong...\n");

        printf("end? (1|0): ");
        scanf("%d",&is_end);
    }
    return 0;
}