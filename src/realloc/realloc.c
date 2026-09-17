#include "../includes/malloc_internal.h"

bool can_extend_block(t_block *block, size_t size) {
  t_block *next_block =
      (t_block *)((char *)block + sizeof(t_block) + block->size);

  return (next_block->freed &&
          next_block->size + block->size + sizeof(t_block) >= size);
}

void *_realloc(void *ptr, size_t size) {
  t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
  t_page *page = (t_page *)((char *)block - sizeof(t_page));
  (void)size;
  (void)page;
  return NULL;
}
