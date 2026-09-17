#include "../includes/malloc_internal.h"
#include <unistd.h>

void ft_putstr_fd(char *str, int fd) {
  size_t i = 0;

  while (str[i]) {
    i++;
  }
  write(fd, str, i);
}

t_block *find_block_with_space(t_page *page, size_t needed) {
  t_block *tmp;

  if (!page) {
    return NULL;
  }

  tmp = page->first;
  if (!tmp && page->size - page->used >= needed + sizeof(t_block)) {
    return create_block(page, needed);
  }
  while (tmp && (!tmp->freed || tmp->size < needed)) {
    tmp = tmp->next;
  }
  if (tmp) {
    return tmp;
  } else if (page->size - page->used >= needed + sizeof(t_block)) {
    return create_block(page, needed);
  }
  return NULL;
}
