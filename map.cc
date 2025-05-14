#include "map.h"
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <iostream>

Map::Map() {
	playerX = WORLD_WIDTH/2;
	playerY = WORLD_HEIGHT/2;

	data.resize(WORLD_HEIGHT, std::vector<int>(WORLD_WIDTH, FLOOR));
	
	for (int y = 0; y < WORLD_HEIGHT; y++) {
		for (int x = 0; x < WORLD_WIDTH; x++) {
			if (y == 0 || x == 0 || y == WORLD_HEIGHT - 1 || x == WORLD_WIDTH - 1) {
				data[y][x] = WALL;
			}
		}
	}
}

void Map::movePlayer(int dy, int dx) {
	int newX = playerX +dx;
	int newY = playerY +dy;

	if (newX >= 0 && newX < WORLD_WIDTH && 
		newY >= 0 && newY < WORLD_HEIGHT && 
		data[newY][newX] != WALL) {
		playerX = newX;
		playerY = newY;
	}
}
		
std::pair<int, int> Map::getPlayerPos() const {
	return {playerX, playerY};
}
void Map::draw(WINDOW* win, int playerX, int playerY) const {
	int winHeight, winWidth;
	getmaxyx(win, winHeight, winWidth);

	int halfHeight = winHeight /2;
	int halfWidth = winWidth /2;

	int cameraY = playerY - halfHeight;
	int cameraX = playerX - halfWidth;

	if (cameraX < 0) cameraX = 0;
	if (cameraY < 0) cameraY = 0;
	if (cameraX + winHeight > WORLD_WIDTH) cameraX = WORLD_WIDTH - winHeight;
	if (cameraY + winWidth > WORLD_HEIGHT) cameraY = WORLD_HEIGHT - winHeight;

	for (int y = 0; y < winHeight; y++) {
		for (int x = 0; x < winWidth; x++) {
			int worldX = cameraX + x;
			int worldY = cameraY + y;
		
			if (worldX >= 0 && worldX < WORLD_WIDTH &&
				worldY >= 0 && worldY < WORLD_HEIGHT) {

			char symbol;
			int color;
					
			if (worldY == playerY && worldX == playerX) {
				symbol = '@';
				color = 3;
			} else {
				switch (data[worldY][worldX]) {
						case FLOOR: symbol = '.'; color = 1; break;
						case WALL: symbol = '#'; color = 2; break;
						default: symbol = '?'; color = 0; break;
					}
				}

				wattron(win, COLOR_PAIR(color));
				mvwaddch(win, y, x, symbol);
				wattroff(win, COLOR_PAIR(color));
			}
		}
	}
}

void Map::drawWorldMap(WINDOW* win, int currentY, int currentX) const {
	int winHeight;
	int winWidth;
	getmaxyx(win, winHeight, winWidth);

	for (int y = 0; y < winHeight; y++){
		for (int x = 0; x < winWidth; x++){
			if (y >= 0 && y <WORLD_HEIGHT && x >= 0 && x < WORLD_WIDTH) {
				char symbol;
				int color;
				if (y == playerY && x == playerX) {
					symbol = '@';
					color = 3;
				} else {
					switch (data[y][x]) {
						case FLOOR: symbol = '.'; color = 1; break;
						case WALL:  symbol = '#'; color = 2; break;
						default:    symbol = '?'; color = 0; break;
				}
			}
			wattron(win, COLOR_PAIR(color));
			mvwaddch(win, y, x, symbol);
			wattroff(win, COLOR_PAIR(color));
			}
		}
	}
}


