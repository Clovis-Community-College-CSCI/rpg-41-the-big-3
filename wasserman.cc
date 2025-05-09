#include <iostream>
#include "inventory.h"

int main() {
    InventoryBST inventory;

    std::cout << "=== Testing Inventory System ===\n";

    // Add items to the inventory
    std::cout << "\nAdding items to the inventory:\n";
    inventory.addItem("Healing Potion", 5, 50, 20, false);
    std::cout << "Added 5 Healing Potions.\n";
    inventory.addItem("Sword", 1, 200, 15, false);
    std::cout << "Added 1 Sword.\n";
    inventory.addItem("Key of Destiny", 1, 0, 0, true);
    std::cout << "Added 1 Key of Destiny (Key Item).\n";
    inventory.addItem("Shield", 1, 150, 10, false);
    std::cout << "Added 1 Shield.\n";

    // Print the inventory
    std::cout << "\nCurrent Inventory:\n";
    inventory.printInventory();

    // Remove some items
    std::cout << "\nRemoving 3 Healing Potions:\n";
    inventory.removeItem("Healing Potion", 3);
    inventory.printInventory();

    // Attempt to remove more Healing Potions than available
    std::cout << "\nAttempting to remove 10 Healing Potions (more than available):\n";
    inventory.removeItem("Healing Potion", 10);
    inventory.printInventory();

    // Attempt to remove a key item
    std::cout << "\nAttempting to remove Key of Destiny (Key Item):\n";
    inventory.removeItem("Key of Destiny", 1);
    inventory.printInventory();

    // Remove all of an item
    std::cout << "\nRemoving all Swords:\n";
    inventory.removeItem("Sword", 1);
    inventory.printInventory();

    // Attempt to remove an item that doesn't exist
    std::cout << "\nAttempting to remove an item that doesn't exist (Magic Wand):\n";
    inventory.removeItem("Magic Wand", 1);
    inventory.printInventory();

    // Add more items to test merging quantities
    std::cout << "\nAdding 3 more Healing Potions (to test merging quantities):\n";
    inventory.addItem("Healing Potion", 3, 50, 20, false);
    inventory.printInventory();

    // Clear the inventory
    std::cout << "\nClearing the inventory (destructor will handle this):\n";
    return 0;
}
