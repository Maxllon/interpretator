#include<read_file.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define uint unsigned

uint fsize(FILE* fl)
{
    int prev=ftell(fl);
    fseek(fl, 0L, SEEK_END);
    int sz=ftell(fl);
    fseek(fl,prev,SEEK_SET); //go back to where we were
    return sz;
}

char* read_file(char* filename)
{
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
    return text;
}