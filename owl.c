#include <ncurses.h>

int main(int argc, char **argv)
{
	int ch;

	/* curses mode initialization */
	initscr();	/* start curses mode */
	raw();		/* disable line buffering */
	noecho();	/* disable echoing - it should be done manually */

	/* enable reading of functional keys (F1, F2, arrow keys, etc */
	keypad (stdscr, TRUE);

	printw("Type any character to see it in bold\n");
	while (1) {
		ch = getch();

		printw("The pressed key is: ");
		attron(A_BOLD);
		printw("%c\n", ch);
		attroff(A_BOLD);
		refresh();

		if (ch == KEY_F(1)) {
			printw("Exiting the editor");
			refresh();
			break;
		}
	}

	getch();

	/* end curses mode */
	endwin();

	return 0;
}
