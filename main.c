#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "error.h"

#define LOG_FILENAME	"owl.log"

#define min(_x, _y)	((_x) < (_y)) ? (_x) : (_y)

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

void screen_update(struct buffer *b)
{
	struct line *line = b->lines;
	int w, h, i, limit;
	int y = 0;

	getmaxyx(stdscr, h, w);

	while (line && (y < h)) {
		move(y, 0);
		for (i = 0, limit = min(w, line->length); i < limit; i++)
			addch(line->data[i]);

		line = line->next;
		y++;
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
