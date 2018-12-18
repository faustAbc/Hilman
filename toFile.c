#include <unistd.h>    // getpid(), getcwd()
#include <sys/types.h> // type definitions, e.g., pid_t
#include <sys/wait.h>  // wait()
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>

int main()
{
  char *fileName = "file";

  int file = open(fileName, O_RDONLY, 0777);

  if (file < 0)
    return 1;


  const int length = 10;
  char buffer[length];
  dup2(file, 0);

  read(STDIN_FILENO, buffer, length);

  printf("line: %s", buffer);

  return 0;
}