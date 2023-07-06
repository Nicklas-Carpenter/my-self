#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#define INPUT_LIM 256

int main(int argc, char *argv[])
{
//printf("%s (line %d): FNV_PRIME = %llx\n", __func__, __LINE__, FNV_PRIME);

	hash_t the_hash;
	if (argc > 1)
		the_hash = hash_str(argv[1], strnlen(argv[1], INPUT_LIM));
	else
		the_hash = hash_str("", 0);

	printf("hash:\t0x%x\n", the_hash);
}
