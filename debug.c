#include <stdio.h>

#include "error.h"

int (*d_puts)(const char *s) = puts;
int (*d_printf)(const char *restrict format, ...) = printf;
