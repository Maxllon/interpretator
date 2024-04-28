#include<read_file.h>

uint fsize(FILE* fl)
{
    int prev=ftell(fl);
    fseek(fl, 0L, SEEK_END);
    int sz=ftell(fl);
    fseek(fl,prev,SEEK_SET); //go back to where we were
    return sz;
}
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* read_file(char* filename)
{
    #ifdef TEST
    filename = concat("D:/PROGRAMMING/projects/interpretator/",filename);
    #endif

    FILE* file = fopen(filename, "r");
    if(file==NULL)
    {
        printf("Cannot open the file: %s\n", filename);
    }
    char* text = malloc(sizeof(char)*fsize(file));
    char c;
    for(uint i = 0; (c = getc(file)) != EOF; i++)
    {
        *(text+i) = c;
    }

    #ifdef TEST
    free(filename);
    #endif
    return text;
}