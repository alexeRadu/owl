#include <stdlib.h>
#include <ncurses.h>

#define WIN_HEIGHT	40
#define WIN_WIDTH	130

#define SQUARE_HEIGHT	4
#define SQUARE_WIDTH	10

struct square {
	int x;
	int y;
	int w;
	int h;
};

WINDOW *win;
struct square *square;

void clear_square(const struct square* square)
{
	int y, x;
	int i, j;

	for (j = 0, y = square->y; j < square->h; j++, y++) {
		x = square->x;
		wmove(win, y, x);
		for (i = 0; i < square->w; i++) {
			waddch(win, ' ');
		}
	}
	wrefresh(win);
}

void draw_line_square(const struct square* square)
{
	int y = square->y;
	int i;

	clear_square(square);

	mvwaddch(win, y, square->x, ACS_ULCORNER);
	for (i = 0; i < (square->w - 2); i++)
		waddch(win, ACS_HLINE);
	waddch(win, ACS_URCORNER);

	for (i = 0, y++; i < (square->h - 2); i++) {
		mvwaddch(win, y, square->x, ACS_VLINE);
		mvwaddch(win, y++, square->x + square->w - 1, ACS_VLINE);
	}

	mvwaddch(win, y, square->x, ACS_LLCORNER);
	for (i = 0; i < square->w - 2; i++)
		waddch(win, ACS_HLINE);
	waddch(win, ACS_LRCORNER);

	wrefresh(win);
}

void draw_full_square(const struct square *square)
{
	int y, x;
	int i, j;

	for (j = 0, y = square->y; j < square->h; j++, y++) {
		x = square->x;
		wmove(win, y, x);
		for (i = 0; i < square->w; i++) {
			waddch(win, ' ' | A_REVERSE);
		}
	}
	wrefresh(win);
}

void square_move_left(struct square *square)
{
	clear_square(square);
	square->x--;
	draw_line_square(square);
}

void square_move_right(struct square *square)
{
	clear_square(square);
	square->x++;
	draw_line_square(square);
}

void square_move_up(struct square *square)
{
	clear_square(square);
	square->y--;
	draw_line_square(square);
}

void square_move_down(struct square *square)
{
	clear_square(square);
	square->y++;
	draw_line_square(square);
}

void new_square()
{
	square = malloc(sizeof(square));

	square->y = (WIN_HEIGHT - SQUARE_HEIGHT) / 2;
	square->x = (WIN_WIDTH - SQUARE_WIDTH) / 2;
	square->w = SQUARE_WIDTH;
	square->h = SQUARE_HEIGHT;
}

int main(int argc, char *argv[])
{
	int winx, winy;
	int c;

	initscr();
	cbreak();
	noecho();

	printw("Press 'q' to exit");
	refresh();

	winy = (LINES - WIN_HEIGHT) / 2;
	winx = (COLS - WIN_WIDTH) /2;
	win = newwin(WIN_HEIGHT, WIN_WIDTH, winy, winx);
	box(win, 0, 0);
	wrefresh(win);

	new_square();

	while ((c = getch()) != 'q') {
		switch(c) {
			case 'n':
				draw_line_square(square);
				break;
			case 'c':
				clear_square(square);
				break;
			case 's':
				draw_full_square(square);
				break;
			case 'h':
				square_move_left(square);
				break;
			case 'j':
				square_move_down(square);
				break;
			case 'k':
				square_move_up(square);
				break;
			case 'l':
				square_move_right(square);
				break;
		}
	}

	free(square);
	endwin();
}
