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
    *size = (size_t)result;
    return 0;
}

Errno read_file_win(const wchar_t* filename, wchar_t** buffer, Arena* ARENA)
{

    FILE* input = _wfopen(filename, L"rb");
    if (input == NULL) {
        wprintf(L"Error: can't open file: %ls\n", filename);
        return errno;
    }
    if (fwide(input, 1) < 0) {
        wprintf(L"Error: failed to set wide orientation\n");
        fclose(input);
        return errno;
    }

    size_t size;
    if (fsize(input, &size) != 0) {
        fclose(input);
        return errno;
    }
    *buffer = (wchar_t*)arena_alloc(ARENA, sizeof(wchar_t) * (size / sizeof(wchar_t) + 1));
    if (*buffer == NULL) {
        fclose(input);
        return ENOMEM;
    }
    size_t read = fread(*buffer, sizeof(wchar_t), size / sizeof(wchar_t), input);
    if (ferror(input)) {
        fclose(input);
        return errno;
    }
    (*buffer)[read] = L'\0';
    if (read > 0 && (*buffer)[0] == 0xFEFF) 
    {
        // Если есть BOM, сдвигаем содержимое
        memmove(*buffer, *buffer + 1, (read - 1) * sizeof(wchar_t));
        (*buffer)[read - 1] = L'\0';
    }
    fclose(input);
    return 0;
}

#endif