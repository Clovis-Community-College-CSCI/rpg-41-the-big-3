#pragma once
#include "Bridgineer.h"
#include <ctime>

class Actor : public HasInitiative { // base class for all moving characters, inherits from HqsInitiative for us to be able to change a hero's speed which will also update in HasInitiative
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
		
		virtual bool is_alive() const { return get_health()> 0; }
};

class Hero : public Actor {
	protected:
		int health = 100;
		int shield = 0;
		int damage = 10; // amount of damage done per hit
		int maxHealth = 100; // originally, max health is the starting health, but it can be increased through finding items in the game, or by picking a hero with more starting health
		int maxShield = 50; // same as above
	public:
		Hero() : Actor("Hero", 20) {}
		// *Note: for a hero to deal damage, they will use a monsters take damage and do something like this:
		// Monster m; hero h; m.take_damage(h.get_damage()); 
		// so damage refers to how much damage they do but take_damage is how they lose health/shield (also players lose shield first, once its 0, then they will lose health)
		virtual int get_health() const override { return health; }
		virtual int get_shield() const { return shield; }
		virtual int get_damage() const { return damage; }
		virtual int get_maxHealth() const { return maxHealth; }
		
		virtual void set_maxHealth() { health = maxHealth; } // this is for if a hero uses a heal item that puts them back to max health
		
		virtual void increase_health(int newHealth) { // increase health for characters to use heals (ex: take a heal that increases health by 50)
			health += newHealth;
			if (health > maxHealth) health = maxHealth;
		}

		virtual void set_maxShield() { shield = maxShield; } // same as the health
		virtual void increase_shield(int newShield) { // same as health
			shield += newShield;
			if (shield > maxShield) shield = maxShield;
		}

		virtual void take_damage(int decr) { // This is for when a player gets hit 
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
				// *IMPORTANT: in main everytime a hero takes damage, it should be checked if health is 0, if so, they need to be dead (delete off database or whatever, cannot be revived)
			}
		}
		virtual void set_damage(int newDamage) { damage = newDamage; } // for if a player picks up an item like a sword that increases their damage that they do per hit by some amount
		virtual void attack(Actor* target) {
			target->take_damage(get_damage());
	}
};

class Tank : public Hero {
	//tanks are just a type of hero that start with more health and have a higher health capacity
	//Pros: start with more health, have a higher health capacity, take .75 damage
	//Cons: heal half as slow with normal heals
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
			int adjusted = decr * 3/4;
			Hero::take_damage(adjusted);
		}
};

class Hunter : public Hero {
	// Hunters are just a type that do more damage; i.e. when they pick up an item, they will get 1.5x damage
	// Pros: 1.5x damage, start with 50 shield
	// Cons: Also take 1.5x damage
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
// *IMPORTANT: in main everytime a hero takes damage, it should be checked if health is 0, if so, they need to be dead (delete off database or whatever, cannot be revived)
};

class Healer : public Hero {
	// Healers are a type that cause all heals for health and shield to regen double the amount
	// Pros: quick healing
	// Cons: does half as much damage
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
	// Wizards use magic to use heals for either health or shield, i.e. if the wizard is max health and no shield and he uses a heal for health, it will go to his shield
	// or if he has like 75 health with max of 100 and uses a health heal of 50 he will then have 100 health and 25 shield
	// also if he uses a set max health heal with max health it will go to his shield instead and vice versa
	// Pros: Special Healing
	// Cons: start with less health and max health
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
	//Ogres are just an easy monster, but they spawm the most
	public:
		Ogre() : Monster("Ogre", 5) {
			health = 30;
			damage = 20;
	}
};

class ColdKiller : public Monster {
	//ColdKillers are just a Cold Killer, do a lot of damage and have low health
	public:
		ColdKiller() : Monster("ColdKiller", 30) {
			damage = 50;
			health = 25;
		}
};

class Chimera : public Monster {
	// Chimeras are a special type of monster that only take damage from every other hit (so like if you hit for 30 damage and then 50, it will only take 50 damage)
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
		hit = !hit;;
	}
};

class Hydra : public Monster {
	// The final boss. ton of health and damage, also every other hit doesnt work like a chimera
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
