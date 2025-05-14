#include <string>
#include <iostream>
#include "Bridges.h"
#include "BSTElement.h"
#include "CircDLelement.h"
#include "/public/read.h"
#include "Bridgineer.h"
#include <cstdlib>
#include <ctime>


using namespace bridges;
using namespace std;

int main() {

	// create Bridges object
	Bridges bridges(1, "ForgotName",
	                "1362253532750");

	//----------------------------------WALL------------------------------------------
	CircDLelement<HasInitiative> *turn = nullptr;
	vector<CircDLelement<HasInitiative>*> storage;
	while (true) {

		string input = read("Enter Name: ");
		if (input == "SNAPSHOT") break;

		int input2 = read("Enter Speed (between 1-40): ");
		if (input2 < 0) break;

		HasInitiative *entity = new HasInitiative(input, input2);

		string label = input + " - Initiative: " + to_string(entity->get_Ini());

		storage.push_back(new CircDLelement<HasInitiative>(*entity, label));

		delete entity;

	}

//Sort by Initiative
	sort(storage.begin(), storage.end(), [](CircDLelement<HasInitiative>* a, CircDLelement<HasInitiative>* b) {
		return (a->getValue().get_Ini() > b->getValue().get_Ini());
	}
	    );


	int sizee = storage.size();

	for (int i = 0; i < sizee; i++) {
		storage[i]->setNext(storage[(i + 1) % storage.size()]);
		storage[i]->setPrev(storage[(((i - 1) + storage.size()) % storage.size())]);
	}

	for (int i = 0; i < sizee;) {
		CircDLelement<HasInitiative>* current = storage[i];
		//Change this part here for whatever needs to be removed.
		if (current->getValue().get_speed() == 39) {

			CircDLelement<HasInitiative> * prev = current->getPrev();
			CircDLelement<HasInitiative> * next = current->getNext();

			if (current == next && current == prev) {
				delete current;
				storage.erase(storage.begin() + i);
				break;
			}
			prev->setNext(next);
			next->setPrev(prev);
			delete current;
			sizee--;
			storage.erase(storage.begin() + i);
		} else {
			i++;
		}
	}
	if (!storage.empty()) {
		turn = storage.at(0);

		bridges.setDataStructure(turn);
		bridges.visualize();
		for (auto x : storage) {
			//  cout << x->getValue().initiative << "\n";
			delete x;
		}
	}
	return 0;
}
