#include <ncurses.h>

#define message(...) \
	{						\
		int r, c;				\
							\
		getmaxyx(stdscr, r, c);			\
		clear();				\
		mvprintw(r >> 1, c >> 1, __VA_ARGS__);	\
		refresh();				\
		getch();				\
		clear();				\
	} while (0)

int main(int argc, char **argv)
{
	int ch;

	/* curses mode initialization */
	initscr();	/* start curses mode */
	raw();		/* disable line buffering */
	noecho();	/* disable echoing - it should be done manually */

	/* enable reading of functional keys (F1, F2, arrow keys, etc */
	keypad (stdscr, TRUE);

	message("Starting the editor");

	printw("Special caracters:");
	printw("\n - upper left corner: ");	addch(ACS_ULCORNER);
	printw("\n - lower left corner: ");	addch(ACS_LLCORNER);
	printw("\n - upper right corner: ");	addch(ACS_URCORNER);
	printw("\n - lower right corner: ");	addch(ACS_LRCORNER);
	printw("\n - tee pointing right: "); 	addch(ACS_RTEE);
	printw("\n - tee pointing left: ");	addch(ACS_LTEE);
	printw("\n - tee pointing up: ");	addch(ACS_BTEE);
	printw("\n - tee pointing down: ");	addch(ACS_TTEE);
	printw("\n - horizontal line: ");	addch(ACS_HLINE);
	printw("\n - vertical line: ");		addch(ACS_VLINE);
	printw("\n - large plus/crossover: ");	addch(ACS_PLUS);
	printw("\n - diamond: ");		addch(ACS_DIAMOND);
	printw("\n - scan line 1: ");		addch(ACS_S1);
	printw("\n - scan line 3: ");		addch(ACS_S3);
	printw("\n - scan line 7: ");		addch(ACS_S7);
	printw("\n - scan line 9: ");		addch(ACS_S9);
	printw("\n");

	printw("Type any character to see it in bold\n");
	refresh();
	while (1) {
		ch = getch();

		printw("The pressed key is: ");
		attron(A_BOLD);
		printw("%c\n", ch);
		attroff(A_BOLD);
		refresh();

		if (ch == KEY_F(1)) {
			message("Exiting the editor");
			break;
		}
	}

	/* end curses mode */
	endwin();

	return 0;
}
