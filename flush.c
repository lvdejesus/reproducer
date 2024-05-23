#include <stdarg.h>
#include <stdio.h>

void printf_hook(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  fflush(0);
  va_end(args);
}

#define printf printf_hook
