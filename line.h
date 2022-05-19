#ifndef LINE_H
#define LINE_H

typedef struct Line {
	size_t  length;
	size_t  capacity;
	struct Line *prev;
	struct Line *next;
	char    data[];
} Line;

Line *line_new(size_t length);
Line *line_new_from_string(const char *text);
Line *line_expand(Line *line, size_t new_size);
Line *line_append_string(Line *line, const char *text);

#endif /* LINE_H */
