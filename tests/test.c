#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_INIT_CAPACITY	128
#define FILE_TMP_BUF_SIZE	256
#define EOL_TOKEN		"\n"
#define EOL_TOKEN_LENGTH	1

#define ENOMEM			1

struct line {
	size_t length;
	size_t capacity;
	struct line *prev;
	struct line *next;
	char data[];
};

struct buffer {
	struct line *lines;
};

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

void screen_update(struct buffer *b)
{
	struct line *line = b->lines;

	while (line) {
		for (int i = 0; i < line->length; i++)
			addch(line->data[i]);

		line = line->next;
	}
	refresh();
}

int main(int argc, char *argv[])
{
	struct buffer buf;
	char *fname;
	FILE *fp;
	int err;

	if (argc != 2) {
		printf("Error: need a file as input parameter\n");
		return -1;
	}

	fname = argv[1];

	fp = fopen(fname, "r");
	if (fp == NULL) {
		printf("Error: unable to open file '%s'\n", fname);
		return -1;
	}

	/* init ncurses */
	initscr();
	noecho();
	cbreak();

	buf.lines = NULL;
	err = buffer_from_file(&buf, fp);
	if (err) {
		printw("Unable to import file\n");
		goto end;
	}

	screen_update(&buf);

end:
	getch();
	endwin();

	return 0;
}
