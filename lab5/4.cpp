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

    // Откроем файл, выйдем с кодом 1 если не получилось
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        puts("Error opening file for reading");
        return 1;
    }

    // Второй аргумент пробразуем в целое число N
    int N = atoi(argv[2]);
    // Здесь будем хранить количество уже выведенных строк в блоке
    int line=0;

    // Будем считывать символы в переменную c, пока не наступит конец ввода
    // Окончание ввода можно инициировать нажатием Ctrl-D
    while( (c = fgetc(f)) != EOF) {
        // Выведем считанный символ на экран
        putc(c, stdout);
        // Если считали символ конца строки и у нас есть ограничение на число
        // строк в блоке, то проверим – не достигли ли мы его
        if (c == '\n' && N>0) {
            line++;
            // Если лимиты достигнуты, то сбросим счетчик строк
            // и считаем символ от пользователя, прежде чем продолжать
            if (line == N) {
                line=0;
                fgetc(stdin);
            }
        }
    }

    // Закроем файл, выйдем с кодом 1 если не получилось
    if (fclose(f) == EOF) {
        puts("Error closing file");
        return 1;
    }

    return 0;
}
