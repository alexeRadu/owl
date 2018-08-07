#include <ncurses.h>

WINDOW *new_win(int h, int w, int y, int x)
{
	WINDOW *win;

	win = newwin(h, w, y, x);
	box(win, 0, 0);
	wrefresh(win);

	return win;
}

void move_win(WINDOW *win, int y, int x)
{
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);

	mvwin(win, y, x);
	box(win, 0, 0);
	wrefresh(win);
}

int main(int argc, char *argv[])
{
	WINDOW *win;
	int w = 10, h = 3;
	int x, y;
	int c;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	printw("Press 'q' to exit");
	refresh();

	y = (LINES - h) / 2;
	x = (COLS - w) / 2;

	win = new_win(h, w, y, x);

	while((c = getch()) != 'q') {
		switch (c) {
			case KEY_LEFT:
				move_win(win, y, --x);
				break;
			case KEY_RIGHT:
				move_win(win, y, ++x);
				break;
			case KEY_UP:
				move_win(win, --y, x);
				break;
			case KEY_DOWN:
				move_win(win, ++y, x);
				break;
		}
	}

	endwin();
	return 0;
}
