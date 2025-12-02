#ifndef MALLOC_H
#define MALLOC_H

#define ALIGNED 8
#define TINY_ALLOC 256
#define TINY_ZONE 25600
#define SMALL_ALLOC 2048
#define SMALL_ZONE 204800

#define PROT_FLAGS PROT_READ | PROT_WRITE
#define MAP_FLAGS MAP_PRIVATE | MAP_ANONYMOUS

#define ALIGN(size) (size + (ALIGNED - size % ALIGNED))

#include <stddef.h>
#include <stdlib.h>

typedef enum e_zone_type {
	TINY,
	SMALL,
	LARGE,
}	t_zone_type;

typedef struct s_block {
  size_t size;
  bool freed;
	t_block *next;
  t_block *prev;
}	t_block;

typedef struct s_page {
	size_t size;
  size_t used;
  void *alloc;
	t_block *start;
  t_block *last;
	t_page *next;
}	t_page;

typedef struct s_memory {
  pthread_mutex mutex;
  t_zone_type type;
  t_page *page;
} t_memory;

typedef struct s_global {
	t_memory tiny;
	t_memory small;
	t_memory large;
}	t_global;

extern t_global global;

#endif
