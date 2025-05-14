#include "game.h"
#include "actor.h"
#include "inventory.h"
//#include "Bridgineer.h"
#include <ncurses.h>
#include <unistd.h>
#include <cstring>

void printInventory(WINDOW* win, BSTNode* node, int& row) {
	if (!node) return;

	printInventory(win, node->left, row);
	wattron(win, COLOR_PAIR(2));
	mvwprintw(win, row++, 4, "%s (x%d)", node->item->name.c_str(), node->item->quantity);
	wattroff(win, COLOR_PAIR(2));
	printInventory(win, node->right, row);
}

void startGame() {
	//initialize
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	set_escdelay(25);

	if (!has_colors()) {
		endwin();
		printf("Your terminal does not support color\n");
		return;
	}
	
	start_color();
	use_default_colors();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, -1);
	init_pair(3, COLOR_GREEN, -1);

	int height, width;
	getmaxyx(stdscr, height, width);

	int win_height = 50;
	int win_width = 150;

	if (height < win_height || width < win_width) {
		endwin();
		printf("Please resize your terminal to at least %dx%d and try again.\n", win_width, win_height);
		return;
	}

	const char* msg = "Game Starting...";
	int msg_y = height/2;
	int msg_x = (width - strlen(msg))/2;

	attron(COLOR_PAIR(3));
	mvprintw(msg_y, msg_x, "%s", msg);
	attroff(COLOR_PAIR(3));
	refresh();
	napms(1000);
	clear();
	refresh();
	
	// Create Game Window
	int starty = (height - win_height) / 2;
	int startx = (width - win_width) / 2;

	WINDOW* gameWin = newwin(win_height, win_width, starty, startx);
	keypad(gameWin, TRUE);
	nodelay(gameWin, TRUE);

	//Game Components 
	Hero* player = new Tank();
	InventoryBST inventory;
	inventory.addItem("Health Potion", 3, 50, 20);
	inventory.addItem("Mana Elixer", 2, 70, 15);
	inventory.addItem("Key of Wisdom", 1, 0, 0, true);

	const char* tabs[] = {"Inventory", "Map", "Character"};
	int currentTab = 0;
	int numTabs = sizeof(tabs) / sizeof(tabs[0]);
	
	enum GameMode { MAIN_GAME, TAB_MENU };
	GameMode currentMode = MAIN_GAME;
	
	bool running = true;
	while (running) {
		werase(gameWin);
		
		wattron(gameWin, COLOR_PAIR(3));
		for (int x = 1; x < win_width - 1; x++) {
			mvwaddch(gameWin, 0, x, '-');
			mvwaddch(gameWin, win_height - 1, x, '-');
		}

		for (int y = 1; y < win_height - 1; y++) {
			mvwaddch(gameWin, y, 0, '|');
			mvwaddch(gameWin, y, win_width - 1, '|');
		}

		mvwaddch(gameWin, 0, 0, '+');
		mvwaddch(gameWin, 0, win_width - 1, '+');
		mvwaddch(gameWin, win_height -1, 0, '+');
		mvwaddch(gameWin, win_height -1, win_width -1, '+');
		wattroff(gameWin, COLOR_PAIR(3));

		//Game View
		if (currentMode == MAIN_GAME) {
			wattron(gameWin, COLOR_PAIR(3));
			mvwprintw(gameWin, 4, 4, "Main Game Running...");
			mvwprintw(gameWin, 6, 4, "Press ESC to open your menu.");
			wattroff(gameWin, COLOR_PAIR(3));
		} else {
		//Tab Menu	
		int tab_row = win_height - 4;
		int spacing = win_width / (numTabs + 1);

		for (int i = 0; i < numTabs; i++) {
			int label_len = strlen(tabs[i]) + 4;
			int tab_col = spacing * (i + 1) - label_len /2;

			if (i == currentTab) 
				wattron(gameWin, COLOR_PAIR(1));
			else 
				wattron(gameWin, COLOR_PAIR(2));

			mvwprintw(gameWin, tab_row, tab_col, "[ %s ]", tabs[i]);

			wattroff(gameWin, COLOR_PAIR(1));
			wattroff(gameWin, COLOR_PAIR(2));
		}

		if (currentTab ==  0) {
			mvwprintw(gameWin, 4, 4, "Inventory:");
			int row = 6;
			printInventory(gameWin, inventory.getRoot(), row);
		}
		else if (currentTab == 1) {
			mvwprintw(gameWin, 4, 4, "Map: (Work in progess)");
		}
		else if (currentTab == 2) {
			mvwprintw(gameWin, 4, 4, "Character Stats:");
			mvwprintw(gameWin, 6, 4, "Health: %d", player->get_health());
			mvwprintw(gameWin, 7, 4, "Shield: %d", player->get_shield());
			mvwprintw(gameWin, 8, 4, "Damage: %d", player->get_damage());
			//mvwprintw(gameWin, 9, 4, "Initiative: %d", playerInit->initiative);
			}
		}

		wrefresh(gameWin);
		
		move(LINES - 1, 0);
		clrtoeol();

		if (currentMode == MAIN_GAME)
			mvprintw(LINES -1, 2, "Press ESC to open menu or press q to quit.");
		else
			mvprintw(LINES - 1, 2, "Use LEFT/RIGHT arrows to switch tabs. Press q to quit.");
		refresh();

		int ch = wgetch(gameWin);

		if (ch != ERR) {
		switch (ch) {
			case 27: //ESC Key
				if (currentMode == MAIN_GAME)
					currentMode = TAB_MENU;
				else
					currentMode = MAIN_GAME;
				break;
			case KEY_LEFT:
				if (currentMode == TAB_MENU)
					currentTab = (currentTab - 1 + numTabs) % numTabs;
				break;
			case KEY_RIGHT:
				if (currentMode == TAB_MENU)
					currentTab = (currentTab + 1) % numTabs;
				break;
			case 'q':
				running = false;
				break;
			}
		}
	}
	delete player;
	delwin(gameWin);
	endwin();
}
