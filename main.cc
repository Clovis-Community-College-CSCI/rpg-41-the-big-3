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

	if (player == nullptr) return 0;

	startGame(player, inventory);
	

	return 0;
}
