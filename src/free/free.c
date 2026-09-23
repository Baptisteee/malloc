#include "../includes/malloc_internal.h"
#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>

t_block *merge_near_freed_blocks(t_block *block) {
  t_block *first = block;
  t_block *last = block;
  size_t new_size = 0;
  while (first->prev && first->prev->freed) {
    first = first->prev;
  }
  for (t_block *tmp = first; tmp && tmp->freed; tmp = tmp->next) {
    new_size += tmp->size + sizeof(t_block);
  }
  while (last->next && last->next->freed) {
    last = last->next;
  }
  new_size -= sizeof(t_block);
  first->size = new_size;
  last->prev = first->prev;
  first->next = last->next;
  if (last->next) {
    last->next->prev = first;
  }
  if (first->prev) {
    first->prev->next = first;
  }
  return first;
}

void remove_page_from_mem(t_memory *memory, t_page *page) {
  if (memory->page == page) {
    memory->page = page->next;
  } else {
    t_page *tmp = memory->page;
    while (tmp->next && tmp->next != page) {
      tmp = tmp->next;
    }
    tmp->next = page->next;
  }
  if (memory->last_page == page) {
    memory->last_page = NULL;
  }
  munmap((char *)page - sizeof(t_page), page->size + sizeof(t_page));
}

void _free(void *ptr) {
  if (!ptr) {
    return;
  }
  if ((size_t)ptr % 8 != 0) {
    ft_putstr_fd("free(): invalid pointer\n", 2);
    raise(SIGABRT);
  }
  t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
  t_page *page = (t_page *)((char *)get_first_block(block) - sizeof(t_page));
  t_memory *memory = get_memory(get_type(block->size));
  if (memory->type == LARGE) {
    remove_page_from_mem(memory, page);
    return;
  }
  block = merge_near_freed_blocks(block);
  block->freed = true;
  if (is_page_empty(page)) {
    remove_page_from_mem(memory, page);
  }
}
