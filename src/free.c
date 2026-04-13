#include "../includes/malloc_internal.h"
#include <sys/mman.h>

void  ft_putstr_fd(char *str, int fd);

void  free(void *ptr) {
  if (!ptr) {
    return;
  }
  t_block *block = (t_block *) ((char *) ptr - sizeof(t_block));
  
  if ((size_t) block % 8 != 0) {
    ft_putstr_fd("free(): invalid pointer\n", 2);
    return;
  }

  if (block->size > SMALL_ALLOC) {
    t_page *page = (t_page *) ((char *) block - sizeof(t_page));
    t_page *tmp = global.large.page;

    if (tmp == page) {
      global.large.page = page->next;
      if (global.large.last_page == page) {
        global.large.last_page = NULL;
      }
    } else {
      while (tmp && tmp->next && tmp->next != page) {
        tmp = tmp->next;
      }
      if (tmp && tmp->next == page) {
        tmp->next = page->next;
        if (global.large.last_page == page) {
          global.large.last_page = tmp;
        }
      }
    }
    
    munmap((void *) page, block->size + sizeof(t_block) + sizeof(t_page));
    return;
  }
  
  block->freed = true;
}
