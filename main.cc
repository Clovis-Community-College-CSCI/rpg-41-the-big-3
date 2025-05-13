#include "menu.h"
#include "game.h"
#include <iostream>

using namespace std;

int main() {

	int choice = showMenu();

	if (choice == 0) {
		startGame();
	}
	return 0;
}
