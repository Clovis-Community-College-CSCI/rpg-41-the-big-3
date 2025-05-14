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

    // If player chose to quit, exit
    if (player == nullptr) {
        std::cout << "Goodbye!\n";
        return 0;
    }

    // Start the main game loop (ncurses UI, map, combat, inventory, etc.)
    startGame(player, inventory);

    // Game over
    std::cout << "Thanks for playing!\n";
    return 0;
}
