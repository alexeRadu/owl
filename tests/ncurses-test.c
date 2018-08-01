#include <ncurses.h>

void draw_hline(unsigned w)
{
	unsigned i;

	for(i = 0; i < w; i++)
		addch(ACS_HLINE);

	refresh();
}

void draw_vline(unsigned h)
{
	unsigned i;
	int x, y;

	getyx(stdscr, y, x);

	for (i = 0; i < h; i++) {
		addch(ACS_VLINE);
		move(++y, x);
	}

	refresh();
}

void draw_box(unsigned w, unsigned h)
{
	int x, y;

	getyx(stdscr, y, x);

	addch(ACS_ULCORNER);
	draw_hline(w - 1);
	addch(ACS_URCORNER);

	move(y + 1, x);
	draw_vline(h - 1);

	move(y + 1, x + w);
	draw_vline(h - 1);

	move(y + h - 1, x);
	addch(ACS_LLCORNER);
	draw_hline(w - 1);
	addch(ACS_LRCORNER);
}

int main()
{
	char c;

	initscr();
	cbreak();
	noecho();

	printw("Hello World !!!\n");
	refresh();

	draw_box(30, 10);

	do {
		c = getch();

		if (c == ' ' || c == '\t' || c == '\n') {
			addch(c);
			continue;
		}

		addch(c | A_BOLD | A_UNDERLINE);
		addch(' ');

	} while(c != 'x');

	endwin();

	return 0;
}
