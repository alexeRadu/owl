#include <stdio.h>
#include <stdarg.h>
#include "log.h"

#define LOG_FILENAME	"owl.log"

void info(const char *msg, ...)
{
	static FILE *fp = NULL;

	if (fp == NULL)
		fp = fopen(LOG_FILENAME, "w+");

	if (fp == NULL)
		return;

	va_list args;
	va_start(args, msg);
	vfprintf(fp, msg, args);
	va_end(args);
}
