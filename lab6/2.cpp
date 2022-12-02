#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

int main( int argc, char *argv[], char *envp[ ])
{
    struct timeval tv;
    time_t timer;
    char buf[9];
    struct tm *tm_info;
    timer = time(NULL);
    pid_t child, child2;

    // Породим дочерний процесс, выйдем если не получилось
    child = fork();
    if (child < 0) {
        puts("Fork failed");
        return 1;
    }

    if (child == 0) {
        // мы внутри дочернего процесса
        // получим текущее время и миллисекунды
        gettimeofday(&tv, NULL);
        tm_info = localtime(&timer);
        // tm_info отформатируем в строку согласно формату "часы:минуты:секунды"
        strftime(buf, 9, "%H:%M:%S", tm_info);
        // миллисекунды добавим из tv
        printf("Child1: Process pid: %d, parent pid: %d, time: %s:%d\n", getpid(), getppid(), buf, tv.tv_usec);
    }
    else {
        // Породим еще один дочерний процесс, выйдем если не получилось
        child2 = fork();
        if (child2 < 0) {
            puts("Fork failed");
            return 1;
        }

        if (child2 == 0) {
            // Выведем такую же информацию ,как и в первом дочернем процессе
            tm_info = localtime(&timer);
            strftime(buf, 9, "%H:%M:%S", tm_info);
            gettimeofday(&tv, NULL);
            printf("Child2: Process pid: %d, parent pid: %d, time: %s:%d\n", getpid(), getppid(), buf, tv.tv_usec);
        }
        else {
            // В родительсео процесса также выведем информацию о времени
            tm_info = localtime(&timer);
            strftime(buf, 9, "%H:%M:%S", tm_info);
            gettimeofday(&tv, NULL);
            printf("Parent: Process pid: %d, parent pid: %d, time: %s:%d\n", getpid(), getppid(), buf, tv.tv_usec);
            // И запустим "ps -x"
            system("ps -x");

            // Подождем дочерние процессы
            int wstatus;
            waitpid(child, &wstatus, 0);
            waitpid(child2, &wstatus, 0);

        }
    }
