// This file is just for me to test the things i made out to ensure its working properly
#include <iostream>
#include <string>
#include "actor.h"
using namespace std;

int main() {
	Hero yo;
	cout << yo.get_health() << endl;
	cout << yo.get_damage() << endl;
	cout << yo.get_maxHealth() << endl;
	Tank t;
	cout << t.get_health() << endl;
	cout << t.get_maxHealth() << endl;
	t.take_damage(40);
	cout << t.get_health() << endl;
	t.increase_health(20);
	cout << t.get_health() << endl;
	t.increase_health(60);
	cout << t.get_health() << endl;
	//yo.set_speed(20);
	cout << yo.get_actorSpeed() << endl;
}
