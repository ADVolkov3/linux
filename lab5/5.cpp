#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main( int argc, char *argv[], char *envp[ ])
{
    char c;

    // Проверка количества аргументов
    if (argc < 3) {
        puts("Missing aruments");
        return 1;
    }

    // Структура для хранения информации о файле
    struct stat st;
    // Откроем файлы, выйдем с кодом 1 если не получилось
    FILE *f = fopen(argv[1], "r");
    FILE *d = fopen(argv[2], "w");
    if (f == NULL) {
        puts("Error opening file for reading");
        return 1;
    }
    if (d == NULL) {
        puts("Error opening file for writing");
        return 1;
    }

    // Считаем инфорацию об исходном файле – нам понадобятся права доступа
    stat(argv[1], &st);

    // Считываем символы по одному из исходного файла и пишем в новый файл
    while( (c = fgetc(f)) != EOF) {
        if (fputc(c, d) == EOF) {
            puts("Error writing to file");
            return 1;
        }
    }

    // Закроем файлы, выйдем с кодом 1 если не получилось
    if (fclose(f) == EOF) {
        puts("Error closing file");
        return 1;
    }
    if (fclose(d) == EOF) {
        puts("Error closing file");
        return 1;
    }
    // Выставим новому файлу те же права доступа, что у исходного
    chmod(argv[2], st.st_mode);

    return 0;
}
