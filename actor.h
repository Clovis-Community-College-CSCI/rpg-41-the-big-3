#pragma once

class Actor { // base class for everything
	protected:
		double x; // x pos
		double y; // y pos
	public:
		virtual double get_x() const { return x; }
		virtual void set_x(double x2) { x = x2; }
		virtual double get_y() const { return y; }
		virtual void set_y(double y2) { y = y2; }
};

class Hero : public Actor {
	protected:
		int health = 100;
		int shield = 0;
		int damage = 10; // amount of damage done per hit
		int maxHealth = 100; // originally, max health is the starting health, but it can be increased through finding items in the game, or by picking a hero with more starting health
		int maxShield = 50; // same as above
	public:
		Hero() {
			health = 100;
			shield = 0;
			damage = 10;
			maxHealth = 100;
			maxShield = 50;
		}
		virtual int get_health() const { return health; }
		virtual int get_shield() const { return shield; }
		virtual int get_damage() const { return damage; }
		virtual int get_maxHealth() const { return maxHealth; }
		virtual void give_maxHealth() { health = maxHealth; } // this is for if a hero uses a heal item that puts them back to max health
		virtual void increase_health(int newHealth) { // increase health instead of set health for characters ot use heals
			health += newHealth;
			if (health > maxHealth) health = maxHealth;
		}
		virtual void take_damage(int decr) { damage -= decr; }
		virtual void give_maxShield() { shield = maxShield; } // same as the health
		virtual void incease_shield(int newShield) { // same as health
			shield += newShield;
			if (shield > maxShield) shield = maxShield;
		}
		virtual void set_damage(int newDamage) { damage = newDamage; } // for if a player picks up an item like a sword that increases their damage that they do per hit by some amount
		virtual void set_maxHealth(int newMax) { maxHealth = newMax; } // for if a player picks up an item to give them more health
		virtual void set_maxShield(int newMax) { maxShield = newMax; } // same as for health
};

class Tank : public Hero {
	//tanks are just a type of hero that start with more health and have a higher health capacity
	//Pros: start with more health, have a higher health capacity, take .75 damage
	//Cons: heal half as slow with normal heals
	public:
		Tank() : Hero() { 
			health = 150;
			maxHealth = 150;
			maxShield = 100;
		}
		void increase_health(int newHealth) override {
			health += (newHealth / 2);
			if (health > maxHealth) health = maxHealth;
		}
		void take_damage(int decr) override { health -= (decr * 3 / 4); }
};
