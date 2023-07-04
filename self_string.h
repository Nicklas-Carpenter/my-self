#ifndef SELF_STRING_H
#define SELF_STRING_H

#include <hash.h>

#define STR_LEN_MAX 4096

typedef struct {
    char *raw_str;
    size_t length;
    hash_t hash;
} SelfString;

SelfString *self_string_create(char *raw_str);
self_string_destroy(SelfString* str);

#endif // SELF_STRING_H
