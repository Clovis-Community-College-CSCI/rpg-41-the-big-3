#include "map.h"
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <iostream>

GameMap::GameMap() : playerPos({width/2, height/2}) {
	grid.resize(height, std::vector<char>(width, '.'));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (rand() % 10 == 0) grid[i][j] = 'T';
			if (rand() % 15 == 0) grid[i][j] = '#';
		}
	}
	if (playerPos.second >= 0 && playerPos.second < height && playerPos.first >= 0 && playerPos.first < width) {
		grid [playerPos.second][playerPos.first] = '.';
	}
}

void GameMap::draw(WINDOW* win) const {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			
			if (y == playerPos.second && x == playerPos.first) {
				if (y + 2 < getmaxy(win) && x + 2 < getmaxx(win)){
					wattron(win, COLOR_PAIR(3));
					mvwaddch(win, y+2, x+2, '@' | A_BOLD | COLOR_PAIR(3));
					wattroff(win, COLOR_PAIR(3));
				} 
			} else {
				if (y + 2 < getmaxy(win) && x + 2 < getmaxx(win)) {
					mvwaddch(win, y+2, x+2, grid[y][x]);
				}
			}
		}
	}
}

void GameMap::drawWorldMap(WINDOW* win, int currentY, int currentX) const {
	int mapHeight = 10;
	int mapWidth = 30;
	int startY = (getmaxy(win) - mapHeight) /2;
	int startX = (getmaxx(win) - mapWidth) /2;

	wattron(win, COLOR_PAIR(3));
	box(win, 0, 0);
	wattroff(win, COLOR_PAIR(3));
	
	if (currentY >= 0 && currentY < height && currentX >= 0 && currentX < width) {
		mvwprintw(win, startY + mapHeight/2, startX + mapWidth/2, "X");
		mvwprintw(win, startY + mapHeight + 1, startX, "Current Postion: (%d,%d)", currentX, currentY);
	}
}

bool GameMap::movePlayer(int dy, int dx) {
	int newX = playerPos.first + dx;
	int newY = playerPos.second + dy;

	if (newX >= 0 && newX < width && newY >= 0 && newY < height && grid[newY][newX] != '#') {
		playerPos.first = newX;
		playerPos.second = newY;
		return true;
	}
	return false;
}

