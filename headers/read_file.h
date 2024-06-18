#ifndef READ_FILE_H
#define READ_FILE_H

#include"includes.h"
#include"arena.h"

static Errno fsize(FILE* file, size_t* size)
{
    long saved = ftell(file);
    if (saved < 0) return errno;
    if (fseek(file, 0, SEEK_END) < 0) return errno;
    long result = ftell(file);
    if (result < 0) return errno;
    if (fseek(file, saved, SEEK_SET) < 0) return errno;
    *size = (size_t) result;
    return 0;
}

Errno read_file(char* filename, wchar** buffer, Arena* ARENA)
{
    FILE* input;
    size_t size;

    char* full_file_name = arena_alloc(ARENA, strlen(filename) + strlen(EXPAN) + 1);
    strcpy(full_file_name, filename);
    strcat(full_file_name, EXPAN);

    if ((input = fopen(full_file_name,"r")) == NULL)
    {
         printf("Error: cant open this file: %s\n", full_file_name);
         EXIT;
    }
    fsize(input, &size);

    *buffer = arena_alloc(ARENA, sizeof(wchar)*size);
    wchar sym;
    size_t i = 0;
    while((sym = fgetwc(input)) != WEOF)
    {
        *(*(buffer)+i) = sym;
        ++i;
    }
    *(*(buffer)+i) = L'\0';

    fclose(input);
    return 0;
}

#endif