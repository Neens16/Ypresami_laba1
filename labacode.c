#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

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
  struct stat *stbuf;
  if (!stat(fileName, stbuf)) {
    if (stbuf->st_mode == 16877) {
      answer = 0;
    } else {
      answer = 1;
    }
  } else {
    printf("stat > %d (%s)\n", stat(fileName, stbuf), fileName);
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
  /*printf("2\n");
  if (!record) {
    printf("END OF %s\n", dirname);
  }
  printf("3\n");*/

  rewinddir(chieldDir);
  closedir(chieldDir);
  return 0;
}

int main(int argc, char *argv[]) {
  /**/
  if (!checkOnDir(argv[1])) {
    printf("YES\n");
    DIR *parentDir = opendir(argv[1]);
    struct dirent *record = readdir(parentDir);
    while (record) {
      printf("%llu %s\n", record->d_ino, record->d_name);
      if (!checkOnDir(record->d_name)) {
        checkDirForDir(record->d_name);
      }
      record = readdir(parentDir);
    }
    rewinddir(parentDir);
    closedir(parentDir);
  } else {
    printf("NO %d\n", checkOnDir(argv[1]));
  }
  return 0;
}