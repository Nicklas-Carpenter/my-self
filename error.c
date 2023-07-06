#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

void pr_error(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	char err_str[1024];
	snprintf(err_str, 1024, format, ap);

	perror(err_str);
	va_end(ap);
}
