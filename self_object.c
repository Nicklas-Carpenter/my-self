#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "self_object.h"
#include "self_string.h"

SelfObject *self_object_create(char *thing)
{
    size_t buf_size = strnlen(thing, STR_LEN_MAX) + 1;
	char *str_buf = malloc(buf_size);
    if (str_buf == NULL)
        return NULL;

	strncpy(str_buf, thing, buf_size);
	SelfObject *obj = malloc(sizeof(SelfObject));
    if (obj == NULL) {
        free(str_buf);
        return NULL;
    }
	obj->thing = str_buf;

	return obj;
}

void self_object_destroy(SelfObject *obj)
{
    free(obj->thing);
    free(obj);
}
