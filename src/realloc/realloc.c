#include "../includes/malloc_internal.h"
#include <signal.h>
#include <stddef.h>
#include <string.h>

bool can_extend_block(t_block *block, size_t size) {
  t_page *page = (t_page *)((char *)get_first_block(block) - sizeof(t_page));

  if (!block->next && page->last == block) {
    return (page->size - page->used <= size);
  }
  return (block->next->freed &&
          block->next->size + block->size + sizeof(t_block) >= size);
}

void *_realloc(void *ptr, size_t size) {
  if (!ptr) {
    return _malloc(ALIGN(size));
  }

  if ((size_t)ptr % 8 != 0) {
    ft_putstr_fd("realloc(): invalid pointer\n", 2);
    raise(SIGABRT);
  }
  size_t aligned_size = ALIGN(size);
  t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
  t_page *page = (t_page *)((char *)block - sizeof(t_page));
  t_memory *memory = get_memory(get_type(aligned_size));
  if (memory->type == LARGE) {
    void *newptr = _malloc(aligned_size);
    memcpy((void *)block + sizeof(t_block), newptr, block->size);
    _free(ptr);
    return newptr;
  }
  if (memory->type != get_type(aligned_size)) {
    void *newptr = _malloc(aligned_size);
    memcpy((void *)block + sizeof(t_block), newptr, block->size);
    _free(ptr);
    return newptr;
  } else {
    // TO DO : change size of block
  }
  (void)page;
  return NULL;
}
