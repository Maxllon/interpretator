#ifndef READ_FILE_H
#define READ_FILE_H

#include "includes.h"

static const wchar ru_list208[] = L"Ё              АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЫЮЯабвгдежзийклмноп";
static const wchar ru_list209[] = L"рстуфхцчшщъыьэюя ё";

static char *cat(char *left, const char *right)
{
    char *temp = (char *)malloc(strlen(left) + strlen(right) + 1);
    for (u8 i = 0; i < strlen(left); i++)
    {
        *(temp + i) = *(left + i);
    }
    for (u8 i = 0; i < strlen(right); i++)
    {
        *(temp + i + strlen(left)) = *(right + i);
    }
    return temp;
}
static long fsize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

wchar *read_file(char *filename)
{
    FILE *file;
    char *f_filename = cat(filename, expan);
    file = fopen(f_filename, "r");
    if (file == NULL)
    {
        printf("Error: cannot open the file: %s", f_filename);
        exit(-1);
    }

    long file_size = fsize(file);
    wchar *text = (wchar *)malloc(2 * file_size + 1);

    char *content = (char *)malloc(file_size + 1);
    content[file_size] = '\0';

    fread(content, 1, file_size, file);
    fclose(file);

    wchar *tx = text;
    for (int i = 0; i < file_size; i++)
    {
        if ((u8)content[i] == 208)
        {
            *tx = ru_list208[(u8)content[i + 1] - 129];
            i++;
        }
        else if ((u8)content[i] == 209)
        {
            *tx = ru_list209[(u8)content[i + 1] - 128];
            i++;
        }
        else
        {
            *tx = content[i];
        }
        tx++;
    }
    *tx = '\0';
    bm_free(content);
    return text;
}

#endif