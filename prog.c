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
  while (1)
  {
    const int MAXDIR = 256;
    char dir[MAXDIR];

    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    char *prog = NULL;
    const int maxArgs = 128;
    char *progArgs[maxArgs];

    char *fileName = NULL;
    char *path = NULL;

    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufsize;
    int s;

    printf("\n>> ");

    read = getline(&line, &len, stdin);

    if (!strcmp(line, "^C\n"))
      return 0;

    prog = strtok(line, " ");

    char *tmp = prog;
    progArgs[0] = tmp;
    int i;
    char *newenviron[] = {NULL};

    for (i = 1; tmp != NULL; i++)
    {
      if (!strcmp(tmp, "<"))
      {
        fileName = strtok(NULL, " ");
        printf("\nfile: %s", fileName);
        tmp = strtok(NULL, " ");
        continue;
      }

      if (!strcmp(tmp, "-p"))
      {
        path = strtok(NULL, " \n");
        printf("\npath: %s;\n", path);
        tmp = strtok(NULL, " ");
        continue;
      }

      progArgs[i] = tmp;
      tmp = strtok(NULL, " ");
    }
    progArgs[i] = NULL;

    pid_t kidpid = fork();
    if (kidpid < 0)
    {
      perror("Internal error: cannot fork.");
      return 1;
    }
    else if (!kidpid)
    {
      if (fileName != NULL)
      {
        int file = open(fileName, O_RDONLY, 0777);
        if (file < 0)
          return 1;
        dup2(file, 0);
      }
      if (path != NULL)
      {
        if (chdir(path) == -1)
          perror("chdir");
      }
      printf("prog: %s", prog);

      execve(prog, progArgs, newenviron);
      perror(line);
      return -1;
    }
    else if (waitpid(kidpid, 0, 0) < 0)
      return -1;
  }

  return 0;
}