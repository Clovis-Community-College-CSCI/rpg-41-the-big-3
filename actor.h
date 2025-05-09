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
			}
			else {
				health -= decr;
				if (health < 0) health = 0;
				// *IMPORTANT: in main everytime a hero takes damage, it should be checked if health is 0, if so, they need to be dead (delete off database or whatever, cannot be revived)
			}
		}
		virtual void set_damage(int newDamage) { damage = newDamage; } // for if a player picks up an item like a sword that increases their damage that they do per hit by some amount
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
		}
		void increase_health(int newHealth) override {
			health += (newHealth / 2);
			if (health > maxHealth) health = maxHealth;
		}
		void take_damage(int decr) override { health -= (decr * 3 / 4); }
};

class Hunter : public Hero {
	// Hunters are just a type that do more damage; i.e. when they pick up an item, they will get 1.5x damage
	// Pros: 1.5x damage, start with 50 shield
	// Cons: Also take 1.5x damage
	public:
		Hunter() : Hero() {
			damage = 15;
			shield = 50;
		}
		void set_damage(int newDamage) override { damage = (newDamage * 1.5); }
		void take_damage(int decr) override {
			if (shield) {
				shield -= (decr * 1.5);
				if (shield < 0) {
					health += shield;
					shield = 0;
					if (health < 0) health = 0;
				}
			}
			else {
				health -= (decr * 1.5);
				if (health < 0) health = 0;
				// *IMPORTANT: in main everytime a hero takes damage, it should be checked if health is 0, if so, they need to be dead (delete off database or whatever, cannot be revived)
			}
		}
};

class Healer : public Hero {
	// Healers are a type that cause all heals for health and shield to regen double the amount
	// Pros: quick healing
	// Cons: does half as much damage
	public: 
		Healer() : Hero() {
			maxHealth = 150;
			maxShield = 100;
		}
		void increase_health(int newHealth) override { 
			health = (newHealth * 2);
			if (health > maxHealth) health = maxHealth;
		}
		void increase_shield(int newShield) override {
			shield = (newShield * 2);
			if (shield > maxShield) shield = maxShield;
		}
		void set_damage(int newDamage) { damage = (newDamage * .5); } 
};

class Wizard : public Hero {
	// Wizards use magic to use heals for either health or shield, i.e. if the wizard is max health and no shield and he uses a heal for health, it will go to his shield
	// or if he has like 75 health with max of 100 and uses a health heal of 50 he will then have 100 health and 25 shield
	// also if he uses a set max health heal with max health it will go to his shield instead and vice versa
	// Pros: Special Healing
	// Cons: start with less health and max health
	Wizard() : Hero() {
		health = 75;
		maxHealth = 75;
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
