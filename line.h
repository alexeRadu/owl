#ifndef LINE_H
#define LINE_H

struct line {
	size_t length;
	size_t capacity;
	struct line *prev;
	struct line *next;
	char data[];
};

struct line *line_new(size_t length);
struct line *line_expand(struct line *line, size_t new_size);
struct line *line_from_string(const char *text);
struct line *line_append(struct line *line, const char *text);

#endif /* LINE_H */
