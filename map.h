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
	void draw(WINDOW* win) const;
	void drawWorldMap(WINDOW* win, int currentY, int currentX) const;
private:
	static const int width = 147;
	static const int height = 47;

	int playerX, playerY;
	int scrollX, scrollY;
	std::vector<std::vector<int>> data;

};

#endif
