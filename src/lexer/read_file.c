#include"read_file.h"
#include<globals.h>

int
read_txt_file(string* buff, const char* file_name)
{
    FILE* file = fopen(file_name, "rb");
    if (!file) return CANNOT_READ_FILE;
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* content = malloc(file_size + 1);
    if (!content) 
    {
        fclose(file);
        LAST_ERROR.type = BAD_ALLOC;
        return BAD_ALLOC;
    }
    size_t bytes_read = fread(content, 1, file_size, file);
    content[bytes_read] = '\0';
    fclose(file);
    utf8_to_utf32(buff, content);
    free(content);
    return ALL_GOOD;
}