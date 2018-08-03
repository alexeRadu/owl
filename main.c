#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "error.h"


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
