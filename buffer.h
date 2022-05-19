#ifndef BUFFER_H
#define BUFFER_H

#include "line.h"


typedef struct {
	Line *lines;
} Buffer;


void buffer_append_line(Buffer *b, Line *line);
int  buffer_from_string(Buffer *b, char *s);
int  buffer_from_file(Buffer *b, FILE *fp);

#endif /* BUFFER_H */
