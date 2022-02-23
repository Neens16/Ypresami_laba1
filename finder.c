#include "finder.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#define KRED "\x1B[31m"
#define KYEL "\x1B[33m"

int checkLevels(char *src) {
  int i = 0, res = 0;
  while (src[i] != '\0') {
    if (src[i++] == '/') res++;
  }
  return res;
}

// Функция checkOnDir проверяет файл с названием dirname на то что это каталог.
// Если это каталог, то функция возвращет 0, Если какой-либо другой файл, то 1,
// если возникает ошибка при получении информации (функция stat), то -1.
int checkOnDir(char *fileName) {
  int answer = 0;
  struct stat stbuf;
  if (!stat(fileName, &stbuf)) {
    if ((stbuf.st_mode & DIRMASK) == DIRMASK) {
      answer = 0;
    } else {
      answer = 1;
    }
  } else {
    answer = -1;
  }
  return answer;
}

int checkDirForDir(char *dirname, int mod) {
  DIR *chieldDir = opendir(dirname);
  struct dirent *record = readdir(chieldDir);
  char str[256], check = 0;
  while (record && !check) {
    strcpy(str, dirname);
    strcat(str, "/");
    strcat(str, record->d_name);
    if (mod && !checkOnDir(str)) {
      check = 1;
    } else if (!mod && record->d_name[0] != '.' && !checkOnDir(str)) {
      if (checkDirForDir(str, 1) == 1) {
        check = 1;
      }
    }
    record = readdir(chieldDir);
  }
  rewinddir(chieldDir);
  closedir(chieldDir);
  return check;
}

result checker(int argc, char *argv[]) {
  result my;
  my.error = 0;
  my.count = 0;
  char str[100];
  int y;
  strcpy(str, argv[1]);
  checkLevels(str);
  if (!checkOnDir(str)) {
    DIR *parentDir = opendir(str);
    if (parentDir != NULL) {
      struct dirent *record = readdir(parentDir);
      while (record) {
        strcpy(str, argv[1]);
        strcat(str, "/");
        strcat(str, record->d_name);
        if (record->d_name[0] != '.' && !checkOnDir(str) &&
            strcmp(record->d_name, "tmp")) {
          y = checkDirForDir(str, 0);
          if (y == 1) {
            strcpy(my.name[my.count], record->d_name);
            my.count += 1;
          }
        }
        record = readdir(parentDir);
      }
      if (my.count == 0) {
        my.error = -1;  // not found
      }
      rewinddir(parentDir);
      closedir(parentDir);

    } else {
      my.error = 2;  // problems with directory
    }

  } else {
    my.error = 1;  // not a directory
  }

  return my;
}

#ifdef MAIN
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
  } else {
    fprintf(stdout, KYEL "В данном каталоге подходящих каталогов не найдено\n");
  }
}

int main(int argc, char *argv[]) {
  result res = checker(argc, argv);
  print_result(res);
  return 0;
}
#endif