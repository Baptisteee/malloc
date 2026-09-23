#ifndef MALLOC_H
#define MALLOC_H

typedef struct s_block t_block;
typedef struct s_page t_page;

#define ALIGNED 8
#define TINY_ALLOC 256
#define TINY_ZONE (100 * (TINY_ALLOC + 32))
#define SMALL_ALLOC 2048
#define SMALL_ZONE (100 * (SMALL_ALLOC + 32))

#define PROT_FLAGS PROT_READ | PROT_WRITE
#define MAP_FLAGS MAP_PRIVATE | MAP_ANONYMOUS
#define MIN_BLOCK_SIZE (sizeof(t_block) + 8)
#define ALIGN(size) (((size) + (ALIGNED - 1)) & ~(ALIGNED - 1))

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum e_zone_type {
  TINY,
  SMALL,
  LARGE,
} t_zone_type;

typedef struct s_block {
  size_t size;
  bool freed;
  t_block *next;
  t_block *prev;
} t_block;

typedef struct s_page {
  size_t size;
  size_t used;
  void *alloc;
  t_block *first;
  t_block *last;
  t_page *next;
} t_page;

typedef struct s_memory {
  pthread_mutex_t mutex;
  t_zone_type type;
  t_page *page;
  t_page *last_page;
} t_memory;

typedef struct s_global {
  t_memory tiny;
  t_memory small;
  t_memory large;
} t_global;

extern t_global global;

void ft_putstr_fd(char *str, int fd);
t_zone_type get_type(size_t size);
t_block *get_first_block(t_block *block);
t_memory *get_memory(t_zone_type type);
void *_malloc(size_t size);
void _free(void *ptr);
bool is_page_empty(t_page *page);
t_block *find_block_with_space(t_page *page, size_t size);
t_block *create_block(t_page *page, size_t size);
#endif
