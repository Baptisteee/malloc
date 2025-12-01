#include "../includes/malloc.h"
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>

t_global global = {NULL, NULL, NULL};

t_zone_type	get_type(size_t size) {
	switch (size) {
		case size > 0 && size <= 256:
			return TINY;
		case size > 256 && size <= 1024:
			return SMALL;
		default:
			return LARGE;
	}
}

void	check_page(t_zone_type type) {
	t_memory *mem;
	if (type == TINY) {
		mem = &global.tiny;
	} else if (type == SMALL) {
		mem = &global.small;
	} else if (type == LARGE) {
		mem = &global.large;
	}
	if (mem == NULL) {
		// init
	}
}

void	*_malloc(size_t size) {
	t_memory memory;
	size_t	aligned_size = ALIGN_16(size);
	t_zone_type type = get_type(aligned_size);
	check_page(type);
}

int	main(int ac, char *av[]) {
}
