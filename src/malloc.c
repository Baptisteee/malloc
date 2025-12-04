#include "../includes/malloc.h"

#include <stdbool.h>
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
	if (size <= TINY_ALLOC) {
			return TINY;
  }
	else if (size > TINY_ALLOC && size <= SMALL_ALLOC) {
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
    default:
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

void  set_last_page(t_memory *mem, t_page *page) {
  t_page *tmp = mem->page;

  if (!tmp) {
    mem->page = page;
    return;
  }
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = page;
}

t_page*	init_page(t_memory *mem, size_t size) {
  size_t page_size = get_size_from_type(mem->type, size) + sizeof(t_page);
  t_page *page = mmap(NULL, page_size, PROT_FLAGS, MAP_FLAGS, 0, 0);
  
  page->alloc = (char *) page + sizeof(t_page);
  page->size = page_size - sizeof(t_page);
  page->used = 0;
  page->start = NULL;
  page->next = NULL;
  page->last = NULL;
  
  return page;
}

// Todo : refactor this shit
// make it search empty block before giving new memory
// Make it better lol 

void	*_malloc(size_t size) {
	size_t	aligned_size = ALIGN(size);
	t_zone_type type = get_type(aligned_size);
  t_memory *mem = get_memory(type);
  t_page *page = mem->page;
  t_block *block;

  if (!mem) {
    return NULL;
  }
  if (!mem->page) {
    page = init_page(mem, size);
    set_last_page(mem, page);
  }
  while (page->used + aligned_size + sizeof(t_block) > page->size && page->next){
    page = page->next;
  }
  if (page->used + aligned_size + sizeof(t_block) > page->size) {
    set_last_page(mem, init_page(mem, size));
    page = page->next;
  }
  block = search_freed_block(page, size); // to implement
  if (!block) {
    block = (t_block *) ((char *) page->alloc + page->used); 
  } else {
    // Resize block & give new pointer
  }
  block->size = sizeof(t_block) + aligned_size;
  block->freed = false;
  block->next = NULL;
  if (!page->start) {
    page->start = block;
    page->last = block;
  } else {
    page->last->next = block;
    page->last = block;
  }
  page->used += block->size;
  return (void *)(block + 1);
}

int	main(int ac, char *av[]) {
  (void) ac;
  (void) av;
  return 1;
}
