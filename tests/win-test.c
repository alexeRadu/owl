#include <ncurses.h>

WINDOW *new_win(int h, int w, int y, int x)
{
	WINDOW *win;

	win = newwin(h, w, y, x);
	box(win, 0, 0);
	wrefresh(win);

	return win;
}

void kill_win(WINDOW *win)
{
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);
	delwin(win);
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

	printw("Press F1 to exit");
	refresh();

	y = (LINES - h) / 2;
	x = (COLS - w) / 2;

	win = new_win(h, w, 10, 10);

	while((c = getch()) != KEY_F(1)) {
		switch (c) {
			case KEY_LEFT:
				kill_win(win);
				win = new_win(h, w, y, --x);
				break;
			case KEY_RIGHT:
				kill_win(win);
				win = new_win(h, w, y, ++x);
				break;
			case KEY_UP:
				kill_win(win);
				win = new_win(h, w, --y, x);
				break;
			case KEY_DOWN:
				kill_win(win);
				win = new_win(h, w, ++y, x);
				break;
		}
	}

	endwin();
	return 0;
}
