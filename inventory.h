#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Represents an individual item in the inventory
class Item {
public:
    std::string name;
    int quantity;
    int value;
    int effect;
    bool isKeyItem;

    Item(const std::string& name, int quantity, int value, int effect, bool isKeyItem = false)
        : name(name), quantity(quantity), value(value), effect(effect), isKeyItem(isKeyItem) {}

    bool operator<(const Item& other) const {
        return name < other.name;
    }

    bool operator==(const Item& other) const {
        return name == other.name;
    }
};

// Represents a node in the binary search tree
class BSTNode {
public:
    std::shared_ptr<Item> item;
    BSTNode* left;
    BSTNode* right;

    BSTNode(std::shared_ptr<Item> item) : item(item), left(nullptr), right(nullptr) {}
};

// Represents the inventory system using a binary search tree
class InventoryBST {
private:
    BSTNode* root;

    void insert(BSTNode*& node, std::shared_ptr<Item> item) {
        if (!node) {
            node = new BSTNode(item);
        } else if (*item < *(node->item)) {
            insert(node->left, item);
        } else if (*(node->item) < *item) {
            insert(node->right, item);
        } else {
            node->item->quantity += item->quantity;
        }
    }

    void inOrderTraversal(BSTNode* node) const {
        if (node) {
            inOrderTraversal(node->left);
            std::cout << node->item->name << " (x" << node->item->quantity << ")\n";
            inOrderTraversal(node->right);
        }
    }

    BSTNode* findMin(BSTNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    BSTNode* remove(BSTNode* node, const std::string& itemName, int quantity) {
        if (!node) return nullptr;

        if (itemName < node->item->name) {
            node->left = remove(node->left, itemName, quantity);
        } else if (node->item->name < itemName) {
            node->right = remove(node->right, itemName, quantity);
        } else {
            if (node->item->isKeyItem) {
                std::cout << "Cannot drop key item: " << itemName << "\n";
                return node;
            }

            if (quantity > node->item->quantity) {
                std::cout << "Warning: Attempting to remove more " << itemName
                          << " than available. Only " << node->item->quantity
                          << " available.\n";
                return node;
            }

            if (quantity == node->item->quantity) {
                if (!node->left) {
                    BSTNode* temp = node->right;
                    delete node;
                    return temp;
                } else if (!node->right) {
                    BSTNode* temp = node->left;
                    delete node;
                    return temp;
                }

                BSTNode* temp = findMin(node->right);
                node->item = temp->item;
                node->right = remove(node->right, temp->item->name, temp->item->quantity);
            } else {
                node->item->quantity -= quantity;
            }
        }
        return node;
    }

    void clear(BSTNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    InventoryBST() : root(nullptr) {}

    ~InventoryBST() {
        clear(root);
    }

    void addItem(const std::string& name, int quantity, int value, int effect, bool isKeyItem = false) {
        auto item = std::make_shared<Item>(name, quantity, value, effect, isKeyItem);
        insert(root, item);
    }

    void removeItem(const std::string& name, int quantity) {
        root = remove(root, name, quantity);
    }

    void printInventory() const {
        std::cout << "Inventory:\n";
        inOrderTraversal(root);
    }
};