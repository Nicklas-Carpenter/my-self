#include "hash.h"
#include <stdio.h>

hash_t hash_str(char *str, size_t length)
{
//printf("\n%s (line %d): str = %s\n", __func__, __LINE__, str);
//printf("%s (line %d): length = %d\n", __func__, __LINE__, length);
	hash_t hash = OFFSET_BASIS;
//printf("%s (line %d): hash = 0x%x\n", __func__, __LINE__, hash);
	for (size_t i = 0; i < length; i++) {
		hash = hash ^ str[i];
//printf("%s (line %d): hash = 0x%x\n", __func__, __LINE__, hash);
		hash = hash * FNV_PRIME;
//printf("%s (line %d): hash = 0x%x\n", __func__, __LINE__, hash);
	}

	return hash;
}
