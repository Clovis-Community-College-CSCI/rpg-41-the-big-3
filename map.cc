#include "map.h"
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <iostream>

Map::Map() {
	playerX = width/2;
	playerY = height/2;
	scrollX = 0;
	scrollY = 0; 

	data.resize(height, std::vector<int>(width, FLOOR));

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (y == 0 || x == 0 || y == height - 1 || x == width - 1) {
				data[y][x] = WALL;
				}
			}
	}
}

void Map::movePlayer(int dy, int dx) {
	int newX = playerX +dx;
	int newY = playerY +dy;

	if (newX >= 0 && newX < width && newY >= 0 && newY < height && data[newY][newX] != WALL) {
		playerX = newX;
		playerY = newY;
	}
}
		
std::pair<int, int> Map::getPlayerPos() const {
	return {playerX, playerY};
}

void Map::draw(WINDOW* win) const {
	int winHeight, winWidth;
	getmaxyx(win, winHeight, winWidth);

	int halfHeight = winHeight /2;
	int halfWidth = winWidth /2;

	int offsetY = playerY - halfHeight;
	int offsetX = playerX - halfWidth;

	if (offsetY < 0) offsetY = 0;
	if (offsetX < 0) offsetX = 0;
	if (offsetY + winHeight > height) offsetY = height - winHeight;
	if (offsetX + winWidth > width) offsetX = width - winWidth;

	for (int y = 0; y < winHeight; y++) {
		for (int x = 0; x < winWidth; x++) {
			int mapY = offsetY + y;
			int mapX = offsetX + x;
		
			if (mapY < 0 || mapY >= height || mapX < 0 || mapX >= width) {
				continue;
			}

			char symbol;
			int color;
					
			if (mapY == playerY && mapX == playerX) {
				symbol = '@';
				color = 3;
			} else {
				switch (data[mapY][mapX]) {
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

void Map::drawWorldMap(WINDOW* win, int currentY, int currentX) const {
	int winHeight;
	int winWidth;
	getmaxyx(win, winHeight, winWidth);

	for (int y = 0; y < winHeight; y++){
		for (int x = 0; x < winWidth; x++){
			int mapY = y;
			int mapX = x;

			if (mapY >= 0 && mapY < height && mapX >= 0 && mapX < width) {
				char symbol;
				int color;
				if (mapY == playerY && mapX == playerX) {
					symbol = '@';
					color = 3;
				} else {
					switch (data[mapY][mapX]) {
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


