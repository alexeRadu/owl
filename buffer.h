#ifndef BUFFER_H
#define BUFFER_H

#include "line.h"


struct buffer {
	struct line *lines;
};


void buffer_append_line(struct buffer *b, struct line *line);
int buffer_from_string(struct buffer *b, char *s);
int buffer_from_file(struct buffer *b, FILE *fp);

#endif /* BUFFER_H */
