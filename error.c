#include <ncurses.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

void error_exit(char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	(void)vfprintf(stderr, format, ap);
	va_end(ap);
	fprintf(stderr, "errno=%d (if applicable)\n", errno);

	exit(1);
}
