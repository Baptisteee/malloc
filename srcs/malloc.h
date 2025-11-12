#ifndef MALLOC_H
#define MALLOC_H

#define ALLIGNED 16
#define SMALL_ALLOC 100
#define SMALL_ZONE 10000
#define MEDIUM_ALLOC 5000
#define MEDIUM_ZONE 500000

enum e_zone_type {
	SMALL,
	MEDIUM,
	BIG;
};

typedef struct s_memory {
	void *mem;
	size_t offset;
}	t_memory;

#endif
