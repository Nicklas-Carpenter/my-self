#include "hash.h"

hash_t hash_str(char *str, size_t length)
{
	hash_t hash = OFFSET_BASIS;
	for (size_t i = 0; i < size; i++) {
		hash = hash ^ str[i];
		hash = hash * FNV_PRIME;
	}

	return hash;
}


