#include "menu.h"
#include "game.h"
#include "actor.h"
#include "inventory.h"
#include <ncurses.h>
#include <string>

using namespace std;

int main() {
	InventoryBST inventory;
	Hero* player = showMenu(inventory);

	if (player != nullptr) {
		startGame(player, inventory);
		delete player;
	}

	return 0;
}
