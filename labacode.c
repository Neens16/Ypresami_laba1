#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define DIRMASK 100000000000000

int writerLevels(int levelCount, char *buff) {
  for (int i = levelCount, position = 0; i > 0; i--, position += 3) {
    buff[position] = '.';
    buff[position + 1] = '.';
    buff[position + 2] = '/';
  }
  return 0;
}

int checkLevels(char *src) {
  int levels = 0;
  while (src[levels * 3 + 2] == '/') {
    levels++;
  }
  return levels;
}

// Функция checkOnDir проверяет файл с названием dirname на то что это каталог.
// Если это каталог, то функция возвращет 0, Если какой-либо другой файл, то 1,
// если возникает ошибка при получении информации (функция stat), то -1.
int checkOnDir(char *fileName) {
  int answer = 0;
  struct stat stbuf;
  printf("%d ----------\n", stat(fileName, &stbuf));
  if (!stat(fileName, &stbuf)) {
    printf("%s tyt %u\n", fileName, stbuf.st_mode);
    if (stbuf.st_mode & DIRMASK == DIRMASK) {
      answer = 0;
    } else {
      answer = 1;
    }
  } else {
    printf("stat > %d (%s)\n", stat(fileName, &stbuf), fileName);
    answer = -1;
  }
  return answer;
}

int checkDirForDir(char *dirname) {
  DIR *chieldDir = opendir(dirname);
  struct dirent *record = readdir(chieldDir);
  // printf("START DFD\n");
  while (record) {
    printf("%s", record->d_name);
    if (!checkOnDir(record->d_name) && dirname[0] != '.') {
      printf("\n\nRESULT >>>> %s\n\n", dirname);
      rewinddir(chieldDir);
      closedir(chieldDir);
      return 0;  ////////////
    }
    record = readdir(chieldDir);
  }
  rewinddir(chieldDir);
  closedir(chieldDir);
  return 0;
}

int main(int argc, char *argv[]) {
  /**/
  struct stat stbuf;
  char str[50];
  strcpy(str, argv[1]);
  //printf("%d (%s)\n", stat(str, &stbuf), str);
  //printf("%d\n", stat("1", stbuf));
  //printf("%d!!!\n", checkOnDir(str));
  //printf("%d  s_isdir\n", S_IFDIR);
  if (!checkOnDir(str)) {
    //printf("YES %d %s\n", stbuf->st_mode, str);
    DIR *parentDir = opendir(str);
    struct dirent *record = readdir(parentDir);
    while (record) {
      printf("%lu %s\n", record->d_ino, record->d_name);
      if (!checkOnDir(record->d_name)) {
        checkDirForDir(record->d_name);
      }
      record = readdir(parentDir);
    }
    rewinddir(parentDir);
    closedir(parentDir);
  } else {
    printf("NO %s\n", str);
  }
  return 0;
}