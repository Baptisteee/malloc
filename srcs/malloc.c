#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>

void	*_malloc(size_t size) {
	void	*retval;

	retval = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,0 ,0);
	if (retval == MAP_FAILED) {
		return NULL;
	}
	return retval;
}

int	main() {
	char *c = _malloc(6);
	if (!c)
		return 1;
	for (size_t i = 0; i < 5; ++i) {
		c[i] = 48;
	}
	c[5] = 0;
	printf("%s\n", c);
}
