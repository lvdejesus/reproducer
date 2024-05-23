#include <stdarg.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void printf_hook(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  fflush(0);
  va_end(args);
}

int main_inner();

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

int main() {
  Options options;
  get_options(&options);

  int p[2];

  if (pipe(p) < 0) {
    return 1;
  }

  pid_t pid = fork();
  if (pid == 0) {
    dup2(p[0], STDIN_FILENO);
    main_inner();
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
          fclose(options.save);
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

#define main main_inner
#define printf printf_hook
