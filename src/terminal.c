#include "terminal.h"
#include <stdio.h>

void get_input(char *buf, size_t buf_size) {
  buf[buf_size - 1] = '\0';
  fgets(buf, buf_size - 1, stdin);
  for (size_t i = 0; i < buf_size; i++) {
    if (buf[i] == '\0') {
      break;
    }
    if (buf[i] == 10) {
      buf[i] = '\0';
    }
  }
}
