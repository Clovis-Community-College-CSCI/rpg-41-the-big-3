#pragma once
#include <vector>
#include <utility>
#include <ncurses.h>

class GameMap{
private:
	std::vector<std::vector<char>> grid;
	std::pair<int, int> playerPos;
	const int width = 30;
	const int height = 20;

public: 
	GameMap();
	void draw(WINDOW* win) const;
	void drawWorldMap(WINDOW* win, int currentY, int currentX) const;
	bool movePlayer(int dy, int dx);
	std::pair<int, int> getPlayerPos() const {return playerPos; }
};

