#include "menu.h"
#include <ncurses.h>
#include <string>

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

        werase(menu);

        wattron(menu, COLOR_PAIR(3));
        wborder(menu, '|','|','-','-','+','+','+','+');
        wattroff(menu, COLOR_PAIR(3));

        for (int i = 0; i < num_options; i++) {
            if (i == choice){
                wattron(menu, COLOR_PAIR(1));
            } else {
                wattron(menu, COLOR_PAIR(2));
            }

            int text_x = (win_width - std::string(options[i]).length()) / 2;
            int text_y = 20 + i * 2;

            mvwprintw(menu, text_y, text_x, "%s", options[i]);

            wattroff(menu, COLOR_PAIR(1));
            wattroff(menu, COLOR_PAIR(2));
        }

        wrefresh(menu);
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
                    running = false;
                break;
            }
        }

    }

	const char* classes[] = {"Tank", "Hunter", "Wizard", "Healer"};
	int classChoice = 0;
	int classCount = 4;
	bool choosing = true;
	
	while (choosing) {
		werase(menu);
        wattron(menu, COLOR_PAIR(3));
        wborder(menu, '|','|','-','-','+','+','+','+');
        wattroff(menu, COLOR_PAIR(3));
		
		mvwprintw(menu, 8, (win_width - 20) / 2, "Choose Your Class:");

		for (int i = 0; i < classCount; i++) {
			if (i == classChoice) wattron(menu, COLOR_PAIR(1));
			else wattron(menu, COLOR_PAIR(2));
			
			int text_x = (win_width - std::string(classes[i]).length()) /2;
			int text_y = 12 + i * 2;
			mvwprintw(menu, text_y, text_x, "%s", classes[i]);

			wattroff(menu, COLOR_PAIR(1));
			wattroff(menu, COLOR_PAIR(2));
		}
	wrefresh(menu);
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


