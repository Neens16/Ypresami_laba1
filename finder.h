#define DIRMASK 0b100000000000000

typedef struct {
  char name[100][100];
  int count;
  int error;
} result;

int checkLevels(char *src);
int checkOnDir(char *fileName);
int checkDirForDir(char *dirname, int mod);
result checker(int argc, char *argv[]);
