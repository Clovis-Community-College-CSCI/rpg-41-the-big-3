#include "map.h"
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <iostream>

Map::Map() {
	playerX = WORLD_WIDTH / 2;
	playerY = WORLD_HEIGHT /2;

	data.assign(WORLD_HEIGHT, std::vector<int>(WORLD_WIDTH, WALL));
	
	for (int y = 0; y < WORLD_HEIGHT; y++) {
		data[y][0] = WALL;
		data[y][WORLD_WIDTH - 1] = WALL;
	}
	for (int x = 0; x < WORLD_WIDTH; x++) {
		data[0][x] = WALL;
		data[WORLD_HEIGHT - 1][x] = WALL;
	}

	generateDungeon(15);

	if (data[playerY][playerX] == WALL) {
		for (int y = 1; y < WORLD_HEIGHT - 1; y++) {
			for (int x = 1; x < WORLD_WIDTH - 1; x++) {
				if (data[y][x] == FLOOR) {
					playerX = x;
					playerY = y;
					return;
				}
			}
		}
	}
}

void Map::movePlayer(int dy, int dx) {
	int newX = playerX +dx;
	int newY = playerY +dy;
	
	//newX = std::max(1, std::min(newX, WORLD_WIDTH - 2));
	//newY = std::max(1, std::min(newY, WORLD_HEIGHT - 2));

	if (newX >= 1 && newX < WORLD_WIDTH - 1 &&
		newY >= 1 && newY < WORLD_HEIGHT - 1 &&
		data[newY][newX] != WALL){
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

	//playerX = std::max(winWidth / 2, std::min(playerX, WORLD_WIDTH - winWidth / 2 - 1));
	//playerY = std::max(winHeight / 2, std::min(playerY, WORLD_HEIGHT - winHeight / 2 -1));

	int halfHeight = winHeight /2;
	int halfWidth = winWidth /2;

	int cameraY = playerY - halfHeight;
	int cameraX = playerX - halfWidth;
	
	cameraX = std::max(0, std::min(cameraX, WORLD_WIDTH - winWidth));
	cameraY = std::max(0, std::min(cameraY, WORLD_HEIGHT - winHeight));

	//if (cameraX < 0) cameraX = 0;
	//if (cameraY < 0) cameraY = 0;
	//if (cameraX + winHeight > WORLD_WIDTH) cameraX = WORLD_WIDTH - winHeight;
	//if (cameraY + winWidth > WORLD_HEIGHT) cameraY = WORLD_HEIGHT - winHeight;

	for (int y = 0; y < winHeight; y++) {
		for (int x = 0; x < winWidth; x++) {
			int worldX = cameraX + x;
			int worldY = cameraY + y;
		
			if (worldX < WORLD_WIDTH && worldY < WORLD_HEIGHT) {
				char symbol;
				int color;
				
				if (worldY == playerY && worldX == playerX) {
					symbol = '@';
					color = 3;
			} 
				else {
				switch (data[worldY][worldX]) {
						case FLOOR: symbol = ' '; color = 1; break;
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
				char symbol = (data[y][x] == WALL) ? '#' : ' ';
				mvwaddch(win, y, x, symbol);
			}
		}
	}
}

//Dungeon Generation
bool Map::canPlaceRoom(const Room& room) {
	if (room.x < 2 || room.y < 2 ||
		room.x + room.width >= WORLD_WIDTH - 12 ||
		room.y + room.height >= WORLD_HEIGHT - 10) {
		return false;
	}

	for(int y = room.y - 1; y < room.y + room.height + 1; y++) {
		for (int x = room.x - 1; x < room.x + room.width + 1; x++) {
			if (data[y][x] != WALL) {
				return false;
			}
		}
	}
	return true;
}

void Map::createRoom(const Room& room) {
	for (int y = room.y; y < room.y + room.height; y++) {
		for(int x = room.x; x < room.x + room.width; x++) {
		data[y][x] = FLOOR;
		}
	}

	for (int y = room.y - 1; y <= room.y + room.height; y++) {
		data[y][room.x - 1] = WALL;
		data[y][room.x + room.width] = WALL;
	}
	for (int x = room.x - 1; x <= room.x + room.width; x++) {
		data[room.y - 1][x] = WALL;
		data[room.y + room.height][x] = WALL;
	}
}

void Map::connectRooms(const Room& a, const Room& b) {

	int x1 = a.x + rand() % a.width;
	int y1 = a.y + rand() % a.height;
	int x2 = b.x + rand() % b.width;
	int y2 = b.y + rand() % b.height;

	while (x1 != x2 || y1 != y2) {
		if (x1 != x2) x1 += (x1 < x2) ? 1 : -1;
		else if (y1 != y2) y1 += (y1 < y2) ? 1 : -1;
		if (data[y1][x1] == WALL) {
		data[y1][x1] = FLOOR;
		}
	}
}

void Map::generateDungeon(int maxRooms) {
	std::vector<Room> rooms;

	for (int i = 0; i < maxRooms; i++) {
		Room room;
		room.width = 10 + rand() % 14; //random width
		room.height = 8 + rand() % 12; //random width
		room.x = 2 + rand() % (WORLD_WIDTH - room.width - 12);
		room.y = 2 + rand() % (WORLD_HEIGHT - room.height - 10);

		if (canPlaceRoom(room)) {
			createRoom(room);
			rooms.push_back(room);
		}
	}

	for (size_t i = 1; i < rooms.size(); i++) {
		connectRooms(rooms[i - 1], rooms[i]);
	}

	if (!rooms.empty()) {
		playerX = rooms[0].x + rooms[0].width /2;
		playerY = rooms[0].y + rooms[0].height /2;
	}
}
