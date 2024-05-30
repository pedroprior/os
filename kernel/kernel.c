#include <kernel/tty.h>
#include <stdio.h>

#if defined(__linux__)
#error "You are not using a cross-compiler!"
#endif
size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void main_kernel(void) {
  terminal_initialize();
  printf("Hello, my first kernel!\n");
}
