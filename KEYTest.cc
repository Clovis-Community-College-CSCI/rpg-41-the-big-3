#include <ncurses.h>

int main() {
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	int ch;
	mvprintw(0,0, "Press arrow keys (ot q to quit):");
	while ((ch = getch()) != 'q') {
		clear();
		mvprintw(1, 0, "You pressed: %d", ch);
		switch (ch) {
			case KEY_UP: mvprintw(2, 0, "Up"); break;
			case KEY_DOWN: mvprintw(2, 0, "Down"); break;
			case KEY_LEFT: mvprintw(2, 0, "Right"); break;
			case KEY_RIGHT: mvprintw(2, 0, "Right"); break;
		}
		refresh();
	}
	endwin();
	return 0;
}
