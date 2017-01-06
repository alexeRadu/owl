#include <ncurses.h>

int main(int argc, char **argv)
{
	initscr();			/* start curses mode */
	printw("Hello World !!!");	/* print to screen */
	refresh();			/* print it on the real screen */
	getch();			/* wait for the user input */
	endwin();			/* end curses mode */

	return 0;
}
