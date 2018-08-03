#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "error.h"


#define FILE_TMP_BUF_SIZE	256
#define EOL_TOKEN		"\n"
#define EOL_TOKEN_LENGTH	1



void buffer_append_line(struct buffer *b, struct line *line)
{
	struct line *l = b->lines;

	if (l == NULL) {
		b->lines = line;
		return;
	}

	while (l->next)
		l = l->next;

	if (strstr((const char *)l->data, EOL_TOKEN)) {
		l->next = line;
		line->prev = l;
		return;
	}

	line_append(l, line->data);
	free(line);
}

int buffer_from_string(struct buffer *b, char *s)
{
	struct line *line;
	char *current, *next;
	char ch_tmp;

	current = s;
	while (1) {
		next = strstr(current, EOL_TOKEN);

		if (next) {
			next += EOL_TOKEN_LENGTH;
			ch_tmp = *next;
			*next = '\0';
		}

		line = line_from_string(current);

		if (next)
			*next = ch_tmp;

		if (line == NULL)
			return -ENOMEM;

		buffer_append_line(b, line);

		if (next == NULL)
			break;

		current = next;
	}

	return 0;
}

int buffer_from_file(struct buffer *b, FILE *fp)
{
	char tmp[FILE_TMP_BUF_SIZE];
	size_t len;
	int ret = 0;

	while(1) {
		len = fread(tmp, 1, FILE_TMP_BUF_SIZE - 1, fp);
		tmp[len] = 0;

		if (len == 0)
			break;

		ret = buffer_from_string(b, tmp);
		if (ret)
			break;

	}

	return ret;
}
