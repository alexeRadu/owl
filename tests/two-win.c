#include <ncurses.h>

#define WIN_HEIGHT	40
#define WIN_WIDTH	130

int main(int argc, char *argv[])
{
	WINDOW *win;
	int c;

	initscr();
	cbreak();
	noecho();

	printw("Press 'q' to exit");
	refresh();

	win = newwin(WIN_HEIGHT, WIN_WIDTH,
		     (LINES - WIN_HEIGHT) / 2, (COLS - WIN_WIDTH) / 2);
	box(win, 0, 0);
	wrefresh(win);

	while ((c = getch()) != 'q') {
	}

	endwin();
}
