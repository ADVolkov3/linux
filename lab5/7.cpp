#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
// суммарный размер файлов в исходном каталоге
int total_size = 0;
// суммарный размер файлов и их количество в текущем подкаталоге
int subfolder_size = 0;
int subfolder_files = 0;
// имя файла с максимальным размером в текущем подкаталоге и сам размер
int maxfile_size = 0;
char maxfile_name[4096];

// Рекурсивная функция подсчета файлов в директории
// Суммирует размер файлов в переменную subfolder_size,
// их количество – в subfolder_files,
// и определяет файл с максимальным размером
int calc_sizes_aux(char *path)
{
    struct dirent *dir;

    DIR *d = opendir(path);
    if (d == NULL) {
        puts("Error opening folder");
        puts(path);
        return -1;
    }

    // сканируем директорию поэлементно
    while ((dir = readdir(d)) != NULL) {
        char d_path[4096];
        snprintf(d_path, 4096, "%s/%s", path, dir->d_name);
        // Если текущий элемент – файл, то узнаем его размер
        if(dir-> d_type != DT_DIR) {
            struct stat st;
            stat(d_path, &st);
            // Увеличим счетчик файлов и суммарный размер
            subfolder_files++;
            subfolder_size += st.st_size;
            total_size += st.st_size;
            // Если текущий размер больше известного к данному моменту
            // максимального, то мы нашли новый максимальный
            if (st.st_size > maxfile_size) {
                maxfile_size = st.st_size;
                sprintf(maxfile_name, "%s", dir->d_name);
            }
        }
        else if(dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 ) // директория, отличная от '.' и '..'
        {
            // Для директорий – рекурсивно обработаем
            int res = calc_sizes_aux(d_path);
            if (res < 0) {
                return res;
            }
        }

    }

    return 0;
}

// Функция схожая с calc_sizes_aux(), но вызываемая для исходного каталога.
// Для всех своих подкаталогов, она вызывает calc_sizes_aux(), предварительно
// сбросив счетчики файлов подкаталога и размер максимального файла.
// Поэтому после обработки каждого подкаталога, мы имеем актуальную информацию
// по нему, которую выводим на экран
int calc_sizes_main(char *path, FILE *f)
{
    struct dirent *dir;

    DIR *d = opendir(path);
    if (d == NULL) {
        puts("Error opening folder");
        puts(path);
        return -1;
    }

    while ((dir = readdir(d)) != NULL) {
        char d_path[4096];
        snprintf(d_path, 4096, "%s/%s", path, dir->d_name);
        if(dir-> d_type != DT_DIR) {
            struct stat st;
            stat(d_path, &st);
            total_size += st.st_size;
        }
        else if(dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 ) // директория, отличная от '.' и '..'
        {
            subfolder_size = 0;
            subfolder_files = 0;
            maxfile_size = 0;
            maxfile_name[0] = '\0';
            int res = calc_sizes_aux(d_path);
            if (res < 0) {
                return res;
            }
            char msg[4096];
            sprintf(msg, "Subfoder '%s': size: %d, total files: %d, max file: %s", dir->d_name,  subfolder_size, subfolder_files, maxfile_name);
            puts(msg);
            fputs(msg, f);
        }

    }

    return 0;
}

int main( int argc, char *argv[], char *envp[ ])
{
    char c;

    // Проверка количества аргументов
    if (argc < 3) {
        puts("Missing aruments");
        return 1;
    }

    // Откроем файл, выйдем с кодом 1 если не получилось
    FILE* f = fopen(argv[2], "w");
    if (f == NULL) {
        puts("Error opening file");
        return 1;
    }

    int res = calc_sizes_main(argv[1], f);
    if (res < 0) {
        return res;
    }

    // В конце программы выведем суммарный размер всех файлов – не зря же собирали
    char msg[256];
    sprintf(msg, "Total folder size: %d", total_size);
    puts(msg);
    fputs(msg, f);

    if (fclose(f) == EOF) {
        puts("Error closing file");
        return 1;
    }

    return 0;
}
