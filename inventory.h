#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// The Item class represents an individual item in the inventory.
class Item {
public:
    std::string name;   // Name of the item
    int quantity;       // Quantity of the item
    int value;          // Gold value of the item
    int effect;         // Effect of the item (e.g., healing amount or damage boost)
    bool isKeyItem;     // Whether the item is a key item (cannot be dropped)

    // Constructor to initialize an item with its properties
    Item(const std::string& name, int quantity, int value, int effect, bool isKeyItem = false)
        : name(name), quantity(quantity), value(value), effect(effect), isKeyItem(isKeyItem) {}

    // Operator overload to compare items alphabetically by name
    bool operator<(const Item& other) const {
        return name < other.name;
    }

    // Operator overload to check if two items have the same name
    bool operator==(const Item& other) const {
        return name == other.name;
    }
};

// The BSTNode class represents a node in the binary search tree.
class BSTNode {
public:
    std::shared_ptr<Item> item; // Pointer to the item stored in this node
    BSTNode* left;              // Pointer to the left child node
    BSTNode* right;             // Pointer to the right child node

    // Constructor to initialize a node with an item
    BSTNode(std::shared_ptr<Item> item) : item(item), left(nullptr), right(nullptr) {}
};

// The InventoryBST class represents the inventory system using a binary search tree.
class InventoryBST {
private:
    BSTNode* root; // Root node of the binary search tree

    // Helper function to insert an item into the BST
    void insert(BSTNode*& node, std::shared_ptr<Item> item) {
        if (!node) {
            // If the node is null, create a new node with the item
            node = new BSTNode(item);
        } else if (*item < *(node->item)) {
            // If the item is less than the current node's item, insert into the left subtree
            insert(node->left, item);
        } else if (*(node->item) < *item) {
            // If the item is greater than the current node's item, insert into the right subtree
            insert(node->right, item);
        } else {
            // If the item already exists, increase its quantity
            node->item->quantity += item->quantity;
        }
    }

    // Helper function to perform an in-order traversal of the BST
    void inOrderTraversal(BSTNode* node) const {
        if (node) {
            inOrderTraversal(node->left); // Visit the left subtree
            std::cout << node->item->name << " (x" << node->item->quantity << ")\n"; // Print the item
            inOrderTraversal(node->right); // Visit the right subtree
        }
    }

    // Helper function to find the node with the smallest value in the BST
    BSTNode* findMin(BSTNode* node) {
        while (node && node->left) {
            node = node->left; // Traverse to the leftmost node
        }
        return node;
    }

    // Helper function to remove an item from the BST
    BSTNode* remove(BSTNode* node, const std::string& itemName, int quantity) {
        if (!node) return nullptr; // If the node is null, return null

        if (itemName < node->item->name) {
            // If the item name is less than the current node's item name, remove from the left subtree
            node->left = remove(node->left, itemName, quantity);
        } else if (node->item->name < itemName) {
            // If the item name is greater than the current node's item name, remove from the right subtree
            node->right = remove(node->right, itemName, quantity);
        } else {
            // If the item is found
            if (node->item->isKeyItem) {
                // Prevent removal of key items
                std::cout << "Cannot drop key item: " << itemName << "\n";
                return node;
            }

            if (quantity > node->item->quantity) {
                // Warn if trying to remove more than available
                std::cout << "Warning: Attempting to remove more " << itemName
                          << " than available. Only " << node->item->quantity
                          << " available.\n";
                return node;
            }

            if (quantity == node->item->quantity) {
                // If the quantity to remove is equal to the current quantity
                if (!node->left) {
                    // If the node has no left child, replace it with the right child
                    BSTNode* temp = node->right;
                    delete node;
                    return temp;
                } else if (!node->right) {
                    // If the node has no right child, replace it with the left child
                    BSTNode* temp = node->left;
                    delete node;
                    return temp;
                }

                // If the node has two children, replace it with the smallest node in the right subtree
                BSTNode* temp = findMin(node->right);
                node->item = temp->item;
                node->right = remove(node->right, temp->item->name, temp->item->quantity);
            } else {
                // Decrease the quantity of the item
                node->item->quantity -= quantity;
            }
        }
        return node;
    }

    // Helper function to clear the BST (used in the destructor)
    void clear(BSTNode* node) {
        if (node) {
            clear(node->left);  // Clear the left subtree
            clear(node->right); // Clear the right subtree
            delete node;        // Delete the current node
        }
    }

public:
    // Constructor to initialize an empty inventory
    InventoryBST() : root(nullptr) {}

    // Destructor to clear the inventory
    ~InventoryBST() {
        clear(root);
    }

    // Add an item to the inventory
    void addItem(const std::string& name, int quantity, int value, int effect, bool isKeyItem = false) {
        auto item = std::make_shared<Item>(name, quantity, value, effect, isKeyItem);
        insert(root, item);
    }

    // Remove an item from the inventory
    void removeItem(const std::string& name, int quantity) {
        root = remove(root, name, quantity);
    }

    // Print the inventory in alphabetical order
    void printInventory() const {
        std::cout << "Inventory:\n";
        inOrderTraversal(root);
    }
};