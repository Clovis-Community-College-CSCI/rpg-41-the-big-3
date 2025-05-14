#include "menu.h"
#include "game.h"
#include "actor.h"
#include "inventory.h"
#include <iostream>

int main() {
    // Create the main inventory BST
    InventoryBST inventory;

    // Show the menu and let the player pick a hero class, populating inventory with starter items
    Hero* player = showMenu(inventory);

	  if (player != nullptr) {
		  startGame(player, inventory);
		  delete player;
	}
    // Game over
    std::cout << "Thanks for playing!\n";
    return 0;
}
