#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  FILE *save;
  FILE *load;
} Options;

Options *get_options(Options *options) {
  options->save = NULL;
  options->load = NULL;

  int choice;
  printf("[1] Save\n");
  printf("[2] Load\n");
  printf("[3] Save and Load\n");
  scanf("%d", &choice);

  char buf[50];

  switch (choice) {
  case 1:
    printf("Path to save to: ");
    scanf("%s", buf);
    options->save = fopen(buf, "w");
    break;
  case 2:
    printf("Path to load from: ");
    scanf("%s", buf);
    options->load = fopen(buf, "r");
    break;
  case 3:
    printf("Path to save to: ");
    scanf("%s", buf);
    options->save = fopen(buf, "w");

    printf("Path to load from: ");
    scanf("%s", buf);
    options->load = fopen(buf, "r");
    break;
  }

  return options;
}

int main(int argc, char *argv[]) {
  Options options;
  get_options(&options);

  int p[2];

  if (pipe(p) < 0) {
    return 1;
  }

  pid_t pid = fork();
  if (pid == 0) {
    dup2(p[0], STDIN_FILENO);
    char *args[] = {NULL};
    execv(argv[1], args);
    return 0;
  } else {
    char buf;
    if (options.load != NULL) {
      while ((buf = fgetc(options.load)) != EOF) {
        if (options.save != NULL) {
          fwrite(&buf, sizeof(char), 1, options.save);
        }
        write(p[1], &buf, sizeof(char));
      }

      fclose(options.load);
    }

    while (1) {
      read(STDOUT_FILENO, &buf, sizeof(char));
      if (buf != '\0') {
        if (buf == 24) { // ctrl X
          if (options.save != NULL) {
            fclose(options.save);
          }
          break;
        }

        if (options.save != NULL) {
          fwrite(&buf, sizeof(char), 1, options.save);
        }

        write(p[1], &buf, 1);
      }
    }

    kill(pid, SIGKILL);
  }
}
