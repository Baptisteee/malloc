#include "../includes/malloc_internal.h"
#include <unistd.h>

void  ft_putstr_fd(char *str, int fd) {
  size_t  i = 0;

  while (str[i]) {
    i++;
  }
  write(fd, str, i);
}
