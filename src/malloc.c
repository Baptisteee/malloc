#include "../includes/malloc_internal.h"

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

static size_t  get_size_from_type(t_zone_type type, size_t size) {
  switch(type) {
    case TINY:
      return TINY_ZONE;
    case SMALL:
      return SMALL_ZONE;
    default:
      return size;
  }
}

static t_memory *get_memory(t_zone_type type) {
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

static void  set_last_page(t_memory *mem, t_page *page) {
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
 
  if (page == MAP_FAILED) {
    return NULL;
  }
  page->alloc = (char *) page + sizeof(t_page);
  page->size = page_size - sizeof(t_page);
  page->used = 0;
  page->first = NULL;
  page->next = NULL;
  page->last = NULL;
  
  return page;
}

t_block *create_block(t_page *page, size_t size) {
  t_block *block = (t_block *) ((char *) page->alloc + page->used);
  
  block->size = size;
  block->freed = false;
  block->next = NULL;
  block->prev = NULL;
  if (!page->first) {
    page->first = block;
  }
  if (page->last) {
    page->last->next = block;
    block->prev = page->last;
  }
  page->last = block;
  page->used += size + sizeof(t_block);
  return block;
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
  return find_block_with_space(page->next, needed);
}

t_block *create_large_alloc(t_memory *mem, size_t size) {
  t_page *new = init_page(mem, size);
  t_block *block;

  if (!new) {
    return NULL;
  }
  set_last_page(mem, new);
  block = (t_block *) ((char *) page->alloc);
  new->used = size;
  block->size = size;
  block->freed = false;

  return block;
}

void  split_freed_block(t_page *page, t_block *block, size_t new_size) {
  t_block *new = (t_block *) ((char *) block + new_size + sizeof(t_block));

  new->freed = true;
  new->size = block->size - new_size - sizeof(t_block);
  new->next = block->next;
  if (new->next) {
    new->next->prev = new;
  }
  if (page->last == block) {
    page->last = new;
  }
  new->prev = block;
  block->next = new;
  block->freed = false;
  block->size = new_size;
}

void	*_malloc(size_t size) {
	size_t	aligned_size = ALIGN(size);
  t_memory *mem = get_memory(get_type(aligned_size));
  t_page *page = mem->page;
  t_block *block;

  if (!mem) {
    return NULL;
  }
  if (type == LARGE) {
    return (void *) (create_large_alloc(page) + 1);
  }
  if (!page) {
    page = init_page(mem, size);
    if (!page) {
      return NULL;
    }
    set_last_page(mem, page);
  }
  block = find_block_with_space(page, aligned_size);
  if (!block) {
    page = init_page(mem, size);
    if (!page) {
      return NULL;
    }
    set_last_page(mem, page);
    block = create_block(page, aligned_size);
  } else if (block->freed == true) {
    if (block->size - aligned_size >= MIN_BLOCK_SIZE) {
      split_block(block, aligned_size);
    }
    block->freed = false;
  }
  return (void *)(block + 1);
}

int	main(int ac, char *av[]) {
  (void) ac;
  (void) av;
  return 1;
}
