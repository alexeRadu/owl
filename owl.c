#include <ncurses.h>


#define min(a, b) 	((a < b) ? (a) : (b))

#define MSG_BUF_LEN	256
#define MAJOR_VERSION	0
#define MINOR_VERSION	1

#define println_center(row, col, sl, el) 			\
	do {							\
		*el = 0;					\
		mvaddstr(row, col - ((el - sl) >> 1), sl);	\
		row += 1;					\
		sl = el + 1;					\
	} while (0)

static void splash_screen(const char *fmt, ...)
{
	va_list arg;
	int row, col;
	char msg[MSG_BUF_LEN];
	char *sline, *eline; 	/* start, end of line */
	char *b;

	/* copy the input string to buffer */
	va_start(arg, fmt);
	vsnprintf(msg, MSG_BUF_LEN, fmt, arg);
	va_end(arg);
	msg[MSG_BUF_LEN] = 0;

	/* get center screen coordinates */
	getmaxyx(stdscr, row, col);
	row = row >> 1;
	col = col >> 1;
	clear();

	b = sline = msg;
	while (1) {
		if ((*b == ' ')  || (*b == '\t') ||
		    (*b == '\n') || (*b == 0)) {
			if ((b - sline) > (col << 1))
				println_center(row, col, sline, eline);

			eline = b;
		}


		if (*b == 0) {
			println_center(row, col, sline, eline);
			break;
		}

		if (*b == '\n')
			println_center(row, col, sline, eline);

		b++;
	}

	refresh();
	getch();
	clear();
}

int main(int argc, char **argv)
{
	int ch;

	/* curses mode initialization */
	initscr();	/* start curses mode */
	raw();		/* disable line buffering */
	noecho();	/* disable echoing - it should be done manually */

	/* enable reading of functional keys (F1, F2, arrow keys, etc */
	keypad (stdscr, TRUE);

	splash_screen("The OWL editor version %d.%d\nAuthor: Alexe Radu Andrei\n"
			"Press any key to continue", MAJOR_VERSION, MINOR_VERSION);

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
			splash_screen("Goodbye");
			break;
		}
	}

	/* end curses mode */
	endwin();

	return 0;
}
