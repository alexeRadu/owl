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
	struct square *next;
	struct square *prev;
};

WINDOW *win;
struct square *sq_current;

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

void square_new()
{
	struct square *square = malloc(sizeof(struct square));

	square->y = (WIN_HEIGHT - SQUARE_HEIGHT) / 2;
	square->x = (WIN_WIDTH - SQUARE_WIDTH) / 2;
	square->w = SQUARE_WIDTH;
	square->h = SQUARE_HEIGHT;

	if (sq_current == NULL) {
		square->prev = square;
		square->next = square;
		sq_current = square;
	} else {
		square_draw(sq_current);

		square->next = sq_current->next;
		square->prev = sq_current;
		sq_current->next->prev = square;
		sq_current->next = square;
		sq_current = square;
	}

	square_draw_selected(square);
}

void square_delete()
{
	struct square *square;

	if (sq_current == NULL)
		return;

	square = sq_current;

	if (sq_current->next == sq_current) {
		sq_current = NULL;
	} else {
		sq_current->next->prev = sq_current->prev;
		sq_current->prev->next = sq_current->next;
		sq_current = sq_current->next;
	}

	square_clear(square);
	free(square);

	/* This is placed here rather than in the previous if/else so that the
	 * cursor stays behinde the selected square */
	if (sq_current != NULL)
		square_draw_selected(sq_current);
}

void square_select_next()
{
	if (sq_current == NULL)
		return;

	if (sq_current->next == sq_current)
		return;

	square_draw(sq_current);
	square_draw_selected(sq_current->next);
	sq_current = sq_current->next;
}

void square_select_prev()
{
	if (sq_current == NULL)
		return;

	if (sq_current->prev == sq_current)
		return;

	square_draw(sq_current);
	square_draw_selected(sq_current->prev);
	sq_current = sq_current->prev;
}

void square_move_left()
{
	if (sq_current == NULL)
		return;

	square_clear(sq_current);
	sq_current->x--;
	square_draw_selected(sq_current);
}

void square_move_right()
{
	if (sq_current == NULL)
		return;

	square_clear(sq_current);
	sq_current->x++;
	square_draw_selected(sq_current);
}

void square_move_up()
{
	if (sq_current == NULL)
		return;

	square_clear(sq_current);
	sq_current->y--;
	square_draw_selected(sq_current);
}

void square_move_down()
{
	if (sq_current == NULL)
		return;

	square_clear(sq_current);
	sq_current->y++;
	square_draw_selected(sq_current);
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

	sq_current = NULL;

	while ((c = getch()) != 'q') {
		switch(c) {
			case 'n':
				square_new();
				break;
			case 'd':
				square_delete();
				break;
			case 'f':
				square_select_next();
				break;
			case 'b':
				square_select_prev();
				break;
			case 'h':
				square_move_left();
				break;
			case 'j':
				square_move_down();
				break;
			case 'k':
				square_move_up();
				break;
			case 'l':
				square_move_right();
				break;
		}
	}

	endwin();
}
