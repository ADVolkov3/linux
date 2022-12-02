#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main( int argc, char *argv[], char *envp[ ])
{
    char c;

    // Проверка количества аргументов
    if (argc < 2) {
        puts("Missing arument");
        return 1;
    }

    struct dirent *dir;

    // Считаем информацию о текущей директории в переменную d
    DIR *d = opendir(".");
    if (d == NULL) {
        puts("Error opening current folder");
        return 1;
    }

    // Выведем информацию о файлах и директориях в текущей директории
    puts("-- Current folder --");
    while ((dir = readdir(d)) != NULL) {
        puts(dir->d_name);
    }
    if (closedir(dir) < 0) {
        puts("Error closing folder");
        return 1;
    }

    d = opendir(argv[1]);
    if (d == NULL) {
        puts("Error opening folder");
        return 1;
    }

    // Выведем информацию о файлах и директориях в директории,
    // переданной как аргумент
    puts("-- custom folder provided as argument --");
    while ((dir = readdir(d)) != NULL) {
        puts(dir->d_name);
    }
    if (closedir(dir) < 0) {
        puts("Error closing folder");
        return 1;
    }

    return 0;
}
