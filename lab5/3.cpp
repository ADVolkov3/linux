#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[], char *envp[ ])
{
    char c;

    // Проверка количества аргументов
    if (argc < 2) {
        puts("Missing arument");
        return 1;
    }

    // Откроем файл, выйдем с кодом 1 если не получилось
    FILE *f = fopen(argv[1], "w");
    if (f == NULL) {
        puts("Error opening file for writing");
        return 1;
    }

    // Будем считывать символы в переменную c, пока не наступит конец ввода
    // Окончание ввода можно инициировать нажатием Ctrl-D
    // Также мы обрабатываем Ctrl-F (код 6) – если пользователь ввел Ctrl-F,
    // то мы тоже закончим
    while( (c = getc(stdin)) != EOF) {
        if (c == 6) {
            break;
        }
        // Запишем считанный символ в файл, выйдем с кодом 1 при проблемах
        if (fputc(c, f) == EOF) {
            puts("Error writing file");
            return 1;
        }
    }

    // Закроем файл, выйдем с кодом 1 если не получилось
    if (fclose(f) == EOF) {
        puts("Error closing file");
        return 1;
    }

    return 0;
}
