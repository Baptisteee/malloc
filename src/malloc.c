#include "../includes/malloc.h"

#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>

t_global global = {
  .tiny = { 
    .type = TINY,
    .page = NULL,
    .mutex = PTHREAD_MUTEX_INITIALIZER
  },
  .small = {
    .type = SMALL,
    .page = NULL,
    .mutex = PTHREAD_MUTEX_INITIALIZER
  },
  .large = {
    .type = LARGE,
    .page = NULL,
    .mutex = PTHREAD_MUTEX_INITIALIZER
  }
};

t_zone_type	get_type(size_t size) {
	if (size > 0 && size <= 256) {
			return TINY;
  }
	else if (size > 256 && size <= 1024) {
			return SMALL;
  }
  return LARGE;
}

size_t  get_size_from_type(t_zone_type type, size_t size) {
  switch(type) {
    case TINY:
      return 25600;
    case SMALL:
      return 204800;
    case LARGE:
      return size;
  }
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

void	init_page(t_memory *mem, size_t size) {
  size_t page_size = get_size_from_type(mem->type, size) + sizeof(t_page);
  mem->page = mmap(NULL, page_size, PROT_FLAGS, MAP_FLAGS, 0, 0);
  mem->page->alloc = (char *) mem->page + sizeof(t_page);
  mem->page->size = page_size - sizeof(t_page);
  mem->page->used = 0;
  mem->page->start = NULL;
  mem->page->next = NULL;
  mem->page->last = NULL;
}

void	*_malloc(size_t size) {
	size_t	aligned_size = ALIGN(size);
	t_zone_type type = get_type(aligned_size);
  t_memory *mem = get_memory(type);
  t_block *block;

  if (!mem) {
    return NULL;
  }
  if (!mem->page) {
    init_page(mem, size);
  }
  if (mem->page->used + aligned_size + sizeof(t_block) > mem->page->size) {
    init_page(mem, size); // TO DO : INIT LAST PAGE ALWAYS
  }
  block = (t_block *) ((char *) mem->page->alloc + mem->page->used);
  block->size = sizeof(t_block) + aligned_size;
  block->freed = false;
  block->next = NULL;
  if (!mem->page->start) {
    mem->page->start = block;
    mem->page->last = block;
  } else {
    mem->page->last->next = block;
    mem->page->last = block;
  }
  mem->page->used += block->size;
  // TO DO : replace mem->page by the last possible page
  return (NULL); // TO DO : RETURN VALUE
}

int	main(int ac, char *av[]) {
}
