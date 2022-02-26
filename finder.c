#include "finder.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#define KRED "\x1B[31m"
#define KYEL "\x1B[33m"

// Считает, на сколько уровней отличается входная директория от текущей
// @char *src - входное значение

int countLevels(char *src) {
  int i = 0, level = 0;
  while (src[i] != '\0') {
    if (src[i++] == '/') level++;
  }
  return level;
}

//---------------------------------------------------------------------------------

// Проверяет файл с названием dirname на то что это каталог
// @char *fileName - имя проверяемого файла

int checkOnDir(char *fileName) {
  int answer = 0;
  struct stat stbuf;
  if (!stat(fileName, &stbuf)) {  // Получение информации о файле
    if ((stbuf.st_mode & DIRMASK) == DIRMASK) {
      answer = 0;  // Если это каталог
    } else {
      answer = 1;  // Если не каталог
    }
  } else {
    answer =
        -1;  // если произошла ошибка при получении информации (функция stat)
  }
  return answer;
}

//---------------------------------------------------------------------------------

// Проверяет директорию на наличие в ней директории
// @char *dirname - имя проверяемой директории
// @int mod - переменная режима работы функции (0, 1)

int checkDirForDir(char *dirname, int mod) {
  DIR *childDir = opendir(dirname);
  char check = 0;
  if (childDir) {
    struct dirent *record = readdir(childDir);  //Чтение записи в директории
    char str[256];
    while (record && !check) {
      strcpy(str, dirname);
      strcat(str, "/");
      strcat(str, record->d_name);  // Составление пути до прочитанной записи в
                                    // директории
      if (mod && !checkOnDir(str)) {
        check = 1;  // Если запись оказалась директорией (последний уровень)
      } else if (!mod && record->d_name[0] != '.' &&
                 !checkOnDir(str)) {  //Если запись оказалась директорией
                                      //(предпоследний уровень)
        if (checkDirForDir(str, 1) == 1) {
          check = 1;  //Если для найденной очередной записи в начальной
                      //директории нашлась внутри директория
        }
      }
      record = readdir(childDir);  //Чтение очередной записи в директории
    }
    rewinddir(childDir);  // Перевод указателя каталога на первую запись
    closedir(childDir);
  }
  return check;
}

//---------------------------------------------------------------------------------
// Основная функция
// Выставляет код возникшей ошибки
// @int argc - количество переданных в функцию аргументов
// @char *argv[1] - название директории для проерки

result checker(int argc, char *argv[]) {
  result my;
  my.error = 0;
  my.count = 0;
  char str[100];
  int y;
  if (argc == 1) {
    my.error = 3;  // Нет входного значения
  } else {
    strcpy(str, argv[1]);
    countLevels(str);
    if (!checkOnDir(str)) {  // Проверка, является ли каталогом
      DIR *parentDir = opendir(str);
      if (parentDir != NULL) {
        struct dirent *record = readdir(parentDir);
        while (record) {
          strcpy(str, argv[1]);
          strcat(str, "/");
          strcat(str, record->d_name);  // Составление пути до прочитанной
                                        // записи в директории
          if (record->d_name[0] != '.' && !checkOnDir(str)) {
            y = checkDirForDir(str, 0);  // Если найдена подходящая директория
            if (y == 1) {
              strcpy(my.name[my.count], record->d_name);
              my.count += 1;  // Увеличение счетчика найденных директорий
            }
          }
          record = readdir(parentDir);
        }
        if (my.count == 0) {
          my.error = -1;  // Не найдено ответа (подходящих директорий)
        }
        rewinddir(parentDir);
        closedir(parentDir);

      } else {
        my.error = 2;  // Проблемы с открытием директории
      }

    } else {
      my.error = 1;  // Не является директорией
      //(обрабатывается как название обыкновенных файлов, так и не существующие
      //названия)
    }
  }

  return my;
}

#ifdef MAIN  // Для сборки для ручного тестирования
void print_result(result res) {
  if (!res.error) {
    for (int i = 0; i < res.count; i++) {
      fprintf(stdout, "%s\n", res.name[i]);
    }
  } else if (res.error == 2) {
    fprintf(stderr, KRED
            "Возникла ошибка "
            "при открытии директории\n");
  } else if (res.error == 1) {
    fprintf(stderr, KRED "Директории с таким названием не существует\n");
  } else if (res.error == -1) {
    fprintf(stdout, KYEL "В данном каталоге подходящих каталогов не найдено\n");
  } else {
    fprintf(stderr, KRED "Каталог не задан\n");
  }
}

int main(int argc, char *argv[]) {
  result res = checker(argc, argv);
  print_result(res);
  return 0;
}
#endif