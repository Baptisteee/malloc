#include "../includes/malloc_internal.h"

void  free(void *ptr) {
  if (!ptr) {
    return;
  }
  t_block *block = (*t_block) ((char *) ptr - sizeof(t_block));

  if ((size_t) block % 8 != 0) {
    ft_putstr_fd("free(): invalid pointer\n");
    return;
  }

  if (block->size > SMALL_ALLOC) {
    munmap((void *) block, block->size + sizeof(t_block));
    return;
  }
  
}
