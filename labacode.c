#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIRMASK 100000000000000

// Функция checkOnDir проверяет файл с названием dirname на то что это каталог.
// Если это каталог, то функция возвращет 0, Если какой-либо другой файл, то 1,
// если возникает ошибка при получении информации (функция stat), то -1.
int checkOnDir(char *fileName) {
  int answer = 0;
  struct stat stbuf;
  if (!stat(fileName, &stbuf)) {
    if (stbuf.st_mode & DIRMASK == DIRMASK) {
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
  char str[256], check = 1;
  while (record && check) {
    strcpy(str, dirname);
    strcat(str, "/");
    strcat(str, record->d_name);
    if (mod && !checkOnDir(str)) {
      check = 0;
    } else if (!mod && record->d_name[0] != '.' && !checkOnDir(str)) {
      if (!checkDirForDir(str, 1)) {
        printf("RESULT >>>> %s\n", dirname);
        check = 0;
      }
    }
    record = readdir(chieldDir);
  }
  rewinddir(chieldDir);
  closedir(chieldDir);
  return 0;
}

int main(int argc, char *argv[]) {
  char str[50];
  strcpy(str, argv[1]);
  if (!checkOnDir(str)) {
    DIR *parentDir = opendir(str);
    struct dirent *record = readdir(parentDir);
    while (record) {
      strcpy(str, argv[1]);
      strcat(str, "/");
      strcat(str, record->d_name);
      if (record->d_name[0] != '.' && !checkOnDir(str)) {
        checkDirForDir(str, 0);
      }
      record = readdir(parentDir);
    }
    rewinddir(parentDir);
    closedir(parentDir);
  } else {
    printf("ERROR %s IS NOT DIRECTORY\n", str);
  }
  return 0;
}