#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "self_string.h"

SelfString *self_string_create(char *raw_str)
{
    size_t len = strnlen(raw_str, STR_MAX_LEN);
    char *raw_str_buf = malloc(len);
    memcpy(raw_str_buf, raw_str);

    SelfString *str = malloc(sizeof(SelfString));
    hash_t str_hash = hash_str(raw_str_buf, len);
    *str = {.raw_str = raw_str_buf, length = len, .hash = str_hash};
}

void self_string_destroy(SelfString *str)
{
    str->
