#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( int argc, char *argv[], char *envp[ ])
{
    pid_t child;

    // Ограничение на длину строки
    size_t len=4096;
    char *line = (char*)malloc(len);
    long int ret=0;

    // Будем считывать строку за строкой из стандартного ввода
    while((ret=getline(&line, &len, stdin))>0) {
        // Уберем перевод строки с конца
        if (line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }

        // Породим дочерний процесс, выйдем если не получилось
        child = fork();
        if (child < 0) {
            puts("Fork failed");
            return 1;
        }

        if (child == 0) {
            // мы внутри дочернего процесса
            // Разобьем строку на аргументы, используя пробел как разделитель
            // Имя команды для запуска будет нулевым аргументом
            // all_args будет хранить список аргументов
            char *all_args[4096];
            char* next_arg = strtok (line, " ");
            // номер текущего аргумента
            int arg_idx = 0;
            while (next_arg != NULL) {
                all_args[arg_idx] = (char*)malloc(strlen(next_arg)+1);
                strcpy(all_args[arg_idx], next_arg);
                // вызываем strtok для получения следующего аргумента
                next_arg = strtok (NULL, " ");
                arg_idx++;
            }
            // После последнего аргумента в all_args должен стоять NULL,
            // иначе execvp может работать сранно
            all_args[arg_idx] = NULL;
            // Собственно запустим сформированную команду с аргументами
            // Имя команды - это нулевой аргумент
            if (execvp(all_args[0], all_args) < 0) {
                printf("Exec error");
                return 1;
            }
        }
        else {
            // В родительском процессе ждем ,пока закончится дочерний,
            // только потом продолжим считывание команд
            int wstatus;
            wait(&wstatus);
            // Сообщим, если команда вернула не ноль, но прерываться не будем
            if (wstatus != 0) {
                printf("Children process returned non-zero: %d\n", wstatus);
            }
        }
    }

    free(line);

    return 0;
}
