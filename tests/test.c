#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_INIT_CAPACITY	128
#define FILE_TMP_BUF_SIZE	1024
#define EOF_TOKEN		"\n"

struct line {
	size_t length;
	size_t capacity;
	struct line *next;
	char data[];
};

struct buffer {
	struct line *lines;
};

struct line *line_new(const char *text)
{
	struct line *line;
	size_t length;
	size_t capacity;

	length = strlen(text);
	capacity = LINE_INIT_CAPACITY;

	while (capacity < length)
		capacity = capacity * 2;

	line = malloc(sizeof(struct line) + capacity);
	if (line) {
		line->capacity = capacity;
		line->length = length;
		line->next = NULL;
		memcpy(line->data, text, length);
	}

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

	l->next = line;
}

int buffer_from_file(struct buffer *b, FILE *fp)
{
	char tmp[FILE_TMP_BUF_SIZE];
	struct line *line;
	char *p, *end;
	size_t len;

	len = fread(tmp, 1, FILE_TMP_BUF_SIZE - 1, fp);
	tmp[len] = 0;

	p = tmp;
	while (1) {
		end = strstr(p, EOF_TOKEN);

		if (end)
			*end = 0;

		line = line_new(p);
		if (line == NULL)
			break;

		buffer_append_line(b, line);

		if (end == NULL)
			break;

		p = end + strlen(EOF_TOKEN);
	}


	return 0;
}

void screen_update(struct buffer *b)
{
	struct line *line = b->lines;

	while (line) {
		for (int i = 0; i < line->length; i++)
			addch(line->data[i]);

		addch('\n');

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
