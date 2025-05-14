#include "menu.h"
#include <ncurses.h>
#include <string>

void drawMenu(WINDOW* win, const char* options[], int num_options, int choice, int win_width, const char* header = nullptr) {
        werase(win);
        wattron(win, COLOR_PAIR(3));
        wborder(win, '|','|','-','-','+','+','+','+');
        wattroff(win, COLOR_PAIR(3));
		
		if (header != nullptr) {
			int header_x = (win_width - std::string(header).length()) /2;
			mvwprintw(win, 18, header_x, "%s", header);
		}

        for (int i = 0; i < num_options; i++) {
            if (i == choice){
                wattron(win, COLOR_PAIR(1));
            } else {
                wattron(win, COLOR_PAIR(2));
            }

            int text_x = (win_width - std::string(options[i]).length()) / 2;
            int text_y = 20 + i * 2;

            mvwprintw(win, text_y, text_x, "%s", options[i]);

            wattroff(win, COLOR_PAIR(1));
            wattroff(win, COLOR_PAIR(2));
        }
		wrefresh(win);
}

Hero* showMenu(InventoryBST& inventory) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    //nodelay(stdscr, TRUE);
    //timeout(0);

    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support color\n");
        return nullptr;
    }

    start_color();
    use_default_colors();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_RED, -1);

    int height;
    int width;
    getmaxyx(stdscr, height, width);

    int win_height = 50;
    int win_width = 150;
    int starty = (height - win_height) / 2;
    int startx = (width - win_width) / 2;

    WINDOW* menu = newwin(win_height, win_width, starty, startx);
    keypad(menu, TRUE);

    const char* options[2] = {"Play", "Quit"};
    int choice = 0;
    int num_options = 2;

    bool running = true;
	Hero* selectHero = nullptr;

    while (running) {
		drawMenu(menu, options, num_options, choice, win_width);
        
		int input = wgetch(menu);
        
		//mvprintw(height - 1, 0, "KEY INPUT: %d", input); //debug
        //refresh();
        if (input != ERR) {
        switch (input) {
            case KEY_UP:
                choice = (choice - 1 + num_options) % num_options;
                break;
            case KEY_DOWN:
                choice = (choice +1) % num_options;
                break;
            case 10:
                if (choice == 1) {
					endwin();
					return nullptr;
				}
				running = false;
                break;
            }
        }

    }

	const char* classes[] = {"Tank", "Hunter", "Wizard", "Healer"};
	int classChoice = 0;
	int classCount = 4;
	bool choosing = true;
	
	const char* header = "Choose your class:";
	
	while (choosing) {
		drawMenu(menu, classes, classCount, classChoice, win_width, header);
		int input = wgetch(menu);

    if (input != ERR) {
    	switch (input) {
            case KEY_UP:
                classChoice = (classChoice - 1 + classCount) % classCount;
                break;
            case KEY_DOWN:
                classChoice = (classChoice + 1) % classCount;
                break;
            case 10:
                choosing = false;
                break;
            }
        }
    }

    delwin(menu);
    endwin();

	inventory.addItem("Health Potion", 3, 50, 20);
	inventory.addItem("Mana Elixir", 2, 70, 15);
	
	switch(classChoice) {
		case 0: return new Tank();
		case 1: return new Hunter();
		case 2: return new Wizard();
		default: return new Healer();
	}
}


