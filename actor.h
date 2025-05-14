#pragma once
#include "Bridgineer.h"
#include <ctime>
#include <string>
#include "inventory.h"
// Forward declaration for inventory
class InventoryBST;
class Item;

class Actor : public HasInitiative {
protected:
    double x; // x pos
    double y; // y pos

public:
    Actor(const std::string& name, int speed)
        : HasInitiative(name, speed), x(0), y(0) {}
    virtual ~Actor() {}

    virtual double get_x() const { return x; }
    virtual void set_x(double x2) { x = x2; }
    virtual double get_y() const { return y; }
    virtual void set_y(double y2) { y = y2; }

    virtual std::string get_name() const { return name; }

    virtual int get_health() const = 0;
    virtual void take_damage(int damage) = 0;

    virtual bool is_alive() const { return get_health() > 0; }
};

class Hero : public Actor {
protected:
    int health = 100;
    int shield = 0;
    int damage = 10;
    int maxHealth = 100;
    int maxShield = 50;
public:
    Hero() : Actor("Hero", 20) {}

    virtual int get_health() const override { return health; }
    virtual int get_shield() const { return shield; }
    virtual int get_damage() const { return damage; }
    virtual int get_maxHealth() const { return maxHealth; }

    virtual void set_maxHealth() { health = maxHealth; }

    virtual void increase_health(int newHealth) {
        health += newHealth;
        if (health > maxHealth) health = maxHealth;
    }

    virtual void set_maxShield() { shield = maxShield; }
    virtual void increase_shield(int newShield) {
        shield += newShield;
        if (shield > maxShield) shield = maxShield;
    }

    virtual void take_damage(int decr) {
        if (shield) {
            shield -= decr;
            if (shield < 0) {
                health += shield;
                shield = 0;
                if (health < 0) health = 0;
            }
        } else {
            health -= decr;
            if (health < 0) health = 0;
        }
    }

    virtual void set_damage(int newDamage) { damage = newDamage; }
    virtual void attack(Actor* target) {
        target->take_damage(get_damage());
    }

    // Use an item by name from the inventory
    virtual bool useItem(const std::string& itemName, InventoryBST& inventory) {
        Item* item = inventory.search(itemName);
        if (!item || item->quantity <= 0) return false;

        // Example logic: apply effect based on item name/type
        if (itemName.find("Health") != std::string::npos) {
            increase_health(item->effect);
        } else if (itemName.find("Shield") != std::string::npos) {
            increase_shield(item->effect);
            // Add mana logic if you have it
        }
        // Add more item types/effects as needed

        // Remove one from inventory
        item->quantity--;
        if (item->quantity <= 0) {
            inventory.removeItem(itemName, 0); // Remove node if quantity is zero
        }
        return true;
    }
};

class Tank : public Hero {
public:
    Tank() : Hero() {
        health = 150;
        shield = 50;
        maxHealth = 150;
        maxShield = 100;
        set_speed(10);
    }

    void increase_health(int newHealth) override {
        health += (newHealth / 2);
        if (health > maxHealth) health = maxHealth;
    }
    void take_damage(int decr) override {
        int adjusted = decr * 3 / 4;
        Hero::take_damage(adjusted);
    }
};

class Hunter : public Hero {
public:
    Hunter() : Hero() {
        damage = 15;
        shield = 50;
        set_speed(25);
    }
    void set_damage(int newDamage) override {
        damage = static_cast<int>(newDamage * 1.5);
    }
    void take_damage(int decr) override {
        int adjusted = static_cast<int>(decr * 1.5);
        Hero::take_damage(adjusted);
    }
};

class Healer : public Hero {
public:
    Healer() : Hero() {
        maxHealth = 150;
        maxShield = 100;
        set_speed(15);
    }
    void increase_health(int newHealth) override {
        health = (newHealth * 2);
        if (health > maxHealth) health = maxHealth;
    }
    void increase_shield(int newShield) override {
        shield = (newShield * 2);
        if (shield > maxShield) shield = maxShield;
    }
    void set_damage(int newDamage) override {
        damage = static_cast<int>(newDamage * 0.5);
    }
};

class Wizard : public Hero {
public:
    Wizard() : Hero() {
        health = 75;
        maxHealth = 75;
        set_speed(30);
    }
    void set_maxHealth() override {
        if (health == maxHealth) shield = maxShield;
        else health = maxHealth;
    }
    void set_maxShield() override {
        if (shield == maxShield) health = maxHealth;
        else shield = maxShield;
    }
    void increase_health(int newHealth) override {
        health += newHealth;
        if (health > maxHealth) {
            shield += (health - maxHealth);
            health = maxHealth;
            if (shield > maxShield) shield = maxShield;
        }
    }
    void increase_shield(int newShield) override {
        shield += newShield;
        if (shield > maxShield) {
            health += (shield - maxShield);
            shield = maxShield;
            if (health > maxHealth) health = maxHealth;
        }
    }
};

class Monster : public Actor {
protected:
    int health;
    int damage;

public:
    Monster(const std::string& name, int speed)
        : Actor(name, speed), health(50), damage(20) {}

    virtual int get_health() const override { return health; }
    virtual int get_damage() const { return damage; }

    virtual void take_damage(int dmg) override {
        health -= dmg;
        if (health < 0) health = 0;
    }

    virtual void attack(Actor* target) {
        target->take_damage(damage);
    }
};

class Ogre : public Monster {
public:
    Ogre() : Monster("Ogre", 5) {
        health = 30;
        damage = 20;
    }
};

class ColdKiller : public Monster {
public:
    ColdKiller() : Monster("ColdKiller", 30) {
        damage = 50;
        health = 25;
    }
};

class Chimera : public Monster {
    bool hit = false;
public:
    Chimera() : Monster("Chimera", 15) {
        damage = 30;
        health = 75;
    }
    void take_damage(int newDamage) override {
        if (hit) {
            health -= newDamage;
            if (health < 0) health = 0;
        }
        hit = !hit;
    }
};

class Hydra : public Monster {
    bool hit = false;
public:
    Hydra() : Monster("Hydra", 25) {
        health = 300;
        damage = 75;
    }
    void take_damage(int newDamage) override {
        if (hit) {
            health -= newDamage;
            if (health < 0) health = 0;
        }
        hit = !hit;
    }
};