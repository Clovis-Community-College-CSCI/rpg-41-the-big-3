#ifndef MAP_H
#define MAP_H

#include <vector>
#include <utility>
#include <ncurses.h>

enum TileType {FLOOR, WALL };

class Map{
public: 
	Map();
	void movePlayer(int dy, int dx);
	std::pair<int, int> getPlayerPos() const;
	void draw(WINDOW* win, int playerX, int playerY) const;
	void drawWorldMap(WINDOW* win, int currentY, int currentX) const;
private:
	static const int WORLD_WIDTH = 500;
	static const int WORLD_HEIGHT = 500;
	static const int SCREEN_WIDTH = 150;
	static const int SCREEN_HEIGHT = 50;
	
	int playerX, playerY;
	std::vector<std::vector<int>> data;

};

#endif
