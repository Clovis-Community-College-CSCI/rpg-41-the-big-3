#include <ncurses.h>
#include <stdlib.h>
#include <string>

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		return 1;
	}

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
	keypad(menu, TRUE);
	box(menu, 0, 0);
	refresh();

	const char* options[2] = {"Play", "Quit" };
	int choice = 0;
	int num_options = 2;

	bool running = true;
	
	while (running) {
		for (int i = 0; i < num_options; i++) {
			if (i == choice){
				wattron(menu, COLOR_PAIR(1));
			} else {
				wattron(menu, COLOR_PAIR(2));
			}

			int text_x = (win_width - std::string(options[i]).length()) / 2;
			int text_y = 3 + i * 2;

			mvwprintw(menu, text_y, text_x, "%s", options[i]);
			
			wattroff(menu, COLOR_PAIR(1));
			wattroff(menu, COLOR_PAIR(2));
		}

		wrefresh(menu);
		int input = wgetch(menu);
		//mvprintw(height - 1, 0, "KEY INPUT: %d", input); //debug
		//refresh(); 
		switch (input) {
			case KEY_UP:
				choice = (choice - 1 + num_options) % num_options;
				break;
			case KEY_DOWN:
				choice = (choice +1) % num_options;
				break;
			case 10:
				if (choice == 0) {

					clear();
					mvprintw(height/2, (width-20) /2, "Game Starting...");
					refresh();
					getch();
					running = false;
				} else {
					running = false;
				}
				break;
			}
			werase(menu);
			box(menu, 0, 0);
	}
	
	delwin(menu);
	endwin();
	return 0;
}
