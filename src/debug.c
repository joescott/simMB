#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

void debug_printf(bool debug, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	if(debug)
		vprintf(fmt, ap);
	va_end(ap);
}
