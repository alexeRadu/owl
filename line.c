#include <string.h>
#include <stdlib.h>
#include "line.h"

#define LINE_INIT_CAPACITY		128


struct line *line_new(size_t length)
{
	struct line *line;
	size_t capacity = LINE_INIT_CAPACITY;

	while (capacity < length)
		capacity = capacity * 2;

	line = malloc(sizeof(struct line) + capacity);
	if (line) {
		line->capacity = capacity;
		line->length = 0;
		line->prev = NULL;
		line->next = NULL;
	}

	return line;
}

struct line *line_expand(struct line *line, size_t new_size)
{
	struct line *dst = line;

	if (new_size <= line->capacity)
		return line;

	dst = line_new(new_size);
	if (dst == NULL)
		return NULL;

	dst->length = line->length;
	strcpy(dst->data, line->data);

	dst->prev = line->prev;
	if (line->prev)
		line->prev->next = dst;

	dst->next = line->next;
	if (line->next)
		line->next->prev = dst;

	free(line);

	return dst;
}

struct line *line_from_string(const char *text)
{
	struct line *line;
	size_t length = strlen(text);

	line = line_new(length + 1);
	if (line) {
		line->length = length;
		strcpy(line->data, text);
	}

	return line;
}

struct line *line_append(struct line *line, const char *text)
{
	line = line_expand(line, line->length + strlen(text) + 1);
	if (line == NULL)
		return NULL;

	strcat(line->data, text);
	line->length += strlen(text);

	return line;
}
