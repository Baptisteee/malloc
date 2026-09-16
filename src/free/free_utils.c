#include "../includes/malloc_internal.h"

t_block* get_first_block(t_block* block) {
  t_block* tmp = block;
  while (tmp->prev) {
    tmp = tmp->prev;
  }
  return tmp;
}

t_memory *get_memory(t_zone_type type) {
    switch(type) {
        case TINY:
            return &global.tiny;
        case SMALL:
            return &global.small;
        case LARGE:
            return &global.large;
        default:
            return NULL;
    }
}

t_zone_type	get_type(size_t size) {
	if (size <= TINY_ALLOC) {
			return TINY;
  }
	else if (size > TINY_ALLOC && size <= SMALL_ALLOC) {
			return SMALL;
  }
  return LARGE;
}

bool is_page_empty(t_page* page) {
  t_block* block = page->first;

  while (block) {
    if (!block->freed) {
      return false;
    }
  }
  return true;
}
