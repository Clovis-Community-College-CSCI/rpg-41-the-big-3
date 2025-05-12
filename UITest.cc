#include <ncurses.h>
#include <stdlib.h>

int main(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	start_color();

	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);

	int height;
	int width;

	getmaxyx(stdscr, height, width);
	
	int win_height = 50;
	int win_width = 150;
	int starty = (height - win_height) / 2;
	int startx = (width - win_width) / 2;
	WINDOW* menu = newwin(win_height, win_width, starty, startx);
	box(menu, 0, 0);
	refresh();
	wrefresh(menu);

	const char* options[2] = {"Play", "Quit" };
	int choice = 0;

	while (1) {
		for (int i = 0; i < 2; i++) {
			if (i == choice)
				wattron(menu, COLOR_PAIR(1));
			else
				wattron(menu, COLOR_PAIR(2));

			mvwprintw(menu, 3 + 1 * 2, (win_width - 4) / 2, "%s", options[i]);
			wattroff(menu, COLOR_PAIR(1));
			wattroff(menu, COLOR_PAIR(2));
		}

		wrefresh(menu);
		int input = wgetch(menu);

		switch (input) {
			case KEY_UP:
				choice = (choice - 1+2) % 2;
				break;
			case KEY_DOWN:
				choice = (choice +1) % 2;
				break;
			case 10:
				if (choice == 0) {

					clear();
					mvprintw(height/2, (width-20) /2, "Game Starting...");
					refresh();
					getch();
					endwin();
					return 0;
				} else {
					endwin();
					return 0;
				}
		}
	}

	endwin();
	return 0;
}
