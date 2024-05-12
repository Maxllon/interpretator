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


wchar_t* read_file(char* filename)
{
    #ifdef TEST
    filename = concat("D:/PROGRAMMING/projects/interpretator/",filename);
    #endif

    FILE *file = fopen(filename, "rb"); // Открываем файл в бинарном режиме для чтения

    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END); // Перемещаем указатель в конец файла
    long fileSize = ftell(file); // Получаем размер файла
    fseek(file, 0, SEEK_SET); // Перемещаем указатель обратно в начало файла

    wchar_t* content = malloc(fileSize); // Выделяем память для содержимого файла (+1 для завершающего нуля)

    if (content == NULL) {
        printf("Ошибка выделения памяти.\n");
        fclose(file);
        return NULL;
    }

    fread(content, 2, fileSize / 2, file); // Читаем содержимое файла

    content[fileSize / 2] = '\0'; // Добавляем завершающий нуль-символ

    fclose(file); // Закрываем файл
    #ifdef TEST
    free(filename);
    #endif
    return content;   
}
