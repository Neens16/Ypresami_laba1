#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "finder.h"

int LEVELS = 0;

void checkLevels(char *src){
  int i = 0;
  while(src[i] != '\0') {
    if (src[i++] == '/') LEVELS++;
  }
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
  char str[256], check = 0, *ans = 0;
  while (record && !check) {
    strcpy(str, dirname);
    strcat(str, "/");
    strcat(str, record->d_name);
    if (mod && !checkOnDir(str)) {
      check = 1;
    } else if (!mod && record->d_name[0] != '.' && !checkOnDir(str)) {
      if (checkDirForDir(str, 1) == 1) {
        int i = LEVELS; 
        ans = strtok(dirname, "/");
        while (i > 0) {
          ans = strtok(NULL, "/");
          i--;
        }
        //printf("%s\n", ans);
        check = 2;
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
    struct dirent *record = readdir(parentDir);
    while (record) {
      strcpy(str, argv[1]);
      strcat(str, "/");
      strcat(str, record->d_name);
      if (record->d_name[0] != '.' && !checkOnDir(str) && strcmp(record->d_name, "tmp")) {
        y = checkDirForDir(str, 0);
        if (y == 2) {
          strcpy(my.name[my.count], record->d_name);
          my.count += 1;
          //printf("%s\n", record->d_name);
        }
      }
      record = readdir(parentDir);
    }
    rewinddir(parentDir);
    closedir(parentDir);
  } else {
    my.error = 1;
    //printf("ERROR %s IS NOT DIRECTORY\n", str);
  }
  
  return my;
}


int main(int argc, char *argv[]) {
  result res = checker(argc, argv);
  if (!res.error) {
    for (int i = 0; i < res.count; i++){
      printf("%s\n", res.name[i]);
    }
  } else {
    printf("NAN");
  }
  return 0;
}