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

void square_clear(const struct square* square)
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

void square_draw(const struct square *square)
{
	int y, x;
	int i, j;

	wattron(win, COLOR_PAIR(1));
	for (j = 0, y = square->y; j < square->h; j++, y++) {
		x = square->x;
		wmove(win, y, x);
		for (i = 0; i < square->w; i++) {
			waddch(win, ' ' | A_REVERSE);
		}
	}
	wrefresh(win);
	wattroff(win, COLOR_PAIR(1));
}

void square_draw_selected(const struct square *square)
{
	int xstart = square->x;
	int xend = square->x + square->w - 1;
	int ystart = square->y;
	int yend = square->y + square->h - 1;

	square_draw(square);

	wattron(win, COLOR_PAIR(2));

	mvwaddch(win, ystart, xstart, ACS_ULCORNER);
	mvwaddch(win, ystart, xend, ACS_URCORNER);
	mvwaddch(win, yend, xstart, ACS_LLCORNER);
	mvwaddch(win, yend, xend, ACS_LRCORNER);

	wrefresh(win);
	wattroff(win, COLOR_PAIR(2));
}

void square_move_left(struct square *square)
{
	square_clear(square);
	square->x--;
	square_draw_selected(square);
}

void square_move_right(struct square *square)
{
	square_clear(square);
	square->x++;
	square_draw_selected(square);
}

void square_move_up(struct square *square)
{
	square_clear(square);
	square->y--;
	square_draw_selected(square);
}

void square_move_down(struct square *square)
{
	square_clear(square);
	square->y++;
	square_draw_selected(square);
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
	start_color();

	init_pair(1, COLOR_BLUE, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);

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
				square_draw_selected(square);
				break;
			case 'c':
				square_draw(square);
				break;
			case 's':
				square_draw_selected(square);
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
