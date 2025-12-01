#ifndef MALLOC_H
#define MALLOC_H

#define ALIGNED 16
#define TINY_ALLOC 256
#define TINY_ZONE 25600
#define SMALL_ALLOC 1024
#define SMALL_ZONE 102400

#define ALIGN_16(size) (size + (16 - size % 16))

#include <stddef.h>
#include <stdlib.h>

typedef enum e_zone_type {
	TINY,
	SMALL,
	LARGE,
}	t_zone_type;

typedef struct s_memory {
	void *mem;
	size_t offset;
}	t_memory;

typedef struct s_global {
	t_memory tiny;
	t_memory small;
	t_memory large;
}	t_global;

extern t_global global;

#endif
