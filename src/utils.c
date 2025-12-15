#include "../include/malloc_internal.h"

void  ft_putstr_fd(char *str, int fd) {
  size_t  i = 0;

  while (str[i]) {
    i++;
  }
  write(fd, str, i);
}
