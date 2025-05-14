#include "game.h"
#include "actor.h"
#include "inventory.h"
#include "map.h"
#include <ncurses.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

void printInventory(WINDOW* win, BSTNode* node, int& row) {
	if (!node) return;
	printInventory(win, node->left, row);
	wattron(win, COLOR_PAIR(2));
	mvwprintw(win, row++, 4, "%s (x%d)", node->item->name.c_str(), node->item->quantity);
	wattroff(win, COLOR_PAIR(2));
	printInventory(win, node->right, row);
}

void startCombat(Hero* player, Monster* monster, WINDOW* win) {
	int combat_height = 15;
	int combat_width = 60;
	int starty = (getmaxy(win) - combat_height) / 2;
	int startx = (getmaxx(win) - combat_width) / 2;

	WINDOW* combatWin = derwin(win, combat_height, combat_width, starty, startx);
	wattron(combatWin, COLOR_PAIR(5));
	box(combatWin, 0, 0);
	wattroff(combatWin, COLOR_PAIR(5));

	mvwprintw(combatWin, 1, 2, "BATTLE! %s vs %s", 
					player->get_name().c_str(),
					monster->get_name().c_str());
	wrefresh(combatWin);

	std::vector<Actor*> combatants = {player, monster};
	for (auto c : combatants) c->roll_Ini();
	std::sort(combatants.begin(), combatants.end(), 
			[](Actor* a, Actor* b) {
			return a->get_Ini() > b->get_Ini(); });

	bool combatOver = false;
	while (!combatOver && player->get_health() > 0) {
		for (auto actor : combatants) {
			if (actor->get_health() <= 0) continue;

			werase(combatWin);
			box(combatWin, 0, 0);

			mvwprintw(combatWin, 1, 2, "%s: HP %d | %s: HP %d",
					player->get_name().c_str(), player->get_health(),
					monster->get_name().c_str(), monster->get_health());

			mvwprintw(combatWin, 3, 2, "%s's turn:",
					actor == player ? "Hero" : monster->get_name().c_str());

			if (actor == player) {

				mvwprintw(combatWin, 5, 4, "1. Attack | 2. Item | 3. Flee");
				wrefresh(combatWin);
			bool valid_choice = false;
			while (!valid_choice) {
				int choice = wgetch(win) - '0';
				switch (choice) {
					case 1:
						monster->take_damage(player->get_damage());
						mvwprintw(combatWin, 10, 2, "You deal %d damage!",
								player->get_damage());
						valid_choice = true;
						break;
					case 2:
						mvwprintw(combatWin, 10, 2, "You fumble for your item...");
						wrefresh(combatWin);
						napms(800);
						delwin(combatWin);
						break;
					case 3:
						if (rand() % 5 == 0) {
						mvwprintw(combatWin, 10, 2, "You escaped!");
						wgetch(combatWin);
						napms(800);
						werase(combatWin);
						wrefresh(combatWin);
						delwin(combatWin);
						return;
					} else {
						mvwprintw(combatWin, 10, 2, "You failed to flee!");
						wrefresh(combatWin);
						napms(800);
					}
					break;
					}
				}

			} else {
				player->take_damage(monster->get_damage());
				mvwprintw(combatWin, 5, 2, "%s attacks for %d damage!",
					monster->get_name().c_str(), monster->get_damage());
			}
		
			wrefresh(combatWin);
			napms(1800);

			if (player->get_health() <= 0 || monster->get_health() <= 0){
				combatOver = true;
				break;
			}
		}
	}

	werase(combatWin);
	box(combatWin, 0, 0);
	if (player->get_health() <= 0) {
		mvwprintw(combatWin, 2, 2, "YOU DIED.");
		mvwprintw(combatWin, 4, 2, "Press 'q' to quit...");
		wrefresh(combatWin);
		int ch;
		while (ch = wgetch(combatWin)) {
				if (ch == 'q' || ch == 'Q') {
				delwin(combatWin);
				endwin();
				exit(0);
			}
		}
	} else {
		mvwprintw(combatWin, 2, 2, "Victory! %s is defeated!", monster->get_name().c_str());
		mvwprintw(combatWin, 4, 2, "Press any key to continue...");
		wrefresh(combatWin);
		wgetch(combatWin);
		}
	delwin(combatWin);
}


	

void startGame(Hero* player, InventoryBST& inventory) {
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
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, -1);
	init_pair(4, COLOR_CYAN, -1);
	init_pair(5, COLOR_RED, -1);

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
	Map gameMap;
	bool mapVisible = false;

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
			auto pos = gameMap.getPlayerPos();
			int playerX = pos.first;
			int playerY = pos.second;
			
			for (Monster* m : gameMap.getMonsters()) {
				if (m->get_x() == playerX && m->get_y() == playerY && m->get_health() > 0) {
					startCombat(player, m, gameWin);
					gameMap.draw(gameWin, playerX, playerY);
					break;
				}
			}
			gameMap.draw(gameWin, playerX, playerY);
		}
		else {
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
		//Tab Content
		if (currentTab ==  0) {
			mvwprintw(gameWin, 4, 4, "Inventory:");
			int row = 6;
			printInventory(gameWin, inventory.getRoot(), row);
		}
		else if (currentTab == 1) {
			werase(gameWin);

			gameMap.drawWorldMap(gameWin, player->get_y(), player->get_x());
			wattron(gameWin, COLOR_PAIR(3));
			mvwprintw(gameWin, 3, (win_width - 10) /2, "MINIMAP");
			wattroff(gameWin, COLOR_PAIR(3));
		}
		else if (currentTab == 2) {
			mvwprintw(gameWin, 4, 4, "Character Stats:");
			mvwprintw(gameWin, 6, 4, "Health: %d", player->get_health());
			mvwprintw(gameWin, 7, 4, "Shield: %d", player->get_shield());
			mvwprintw(gameWin, 8, 4, "Damage: %d", player->get_damage());
			}
		}

		wrefresh(gameWin);
		
		move(LINES - 1, 0);
		clrtoeol();
		if (currentMode == MAIN_GAME){
			mvprintw(LINES -1, 2, "Move: Arrow Keys | Menu: ESC | Quit: q");
		} else {
			mvprintw(LINES - 1, 2, "Navigate: LEFT/RIGHT | Close Menu: ESC | Quit: q");
		}
		refresh();

		int ch = wgetch(gameWin);
		if (ch != ERR) {
			switch (ch) {
				case 'w': gameMap.movePlayer(-1, 0); break;
				case 's': gameMap.movePlayer(1, 0); break;
				case 'a': gameMap.movePlayer(0, -1); break;
				case 'd': gameMap.movePlayer(0, 1); break;

				case 'm':
					if (currentMode == MAIN_GAME)
						mapVisible = !mapVisible;
					break;
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
	delwin(gameWin);
	endwin();
}
