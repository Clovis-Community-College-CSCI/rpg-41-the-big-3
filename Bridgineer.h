#include <string>
#include <iostream>
#include "Bridges.h"
#include "BSTElement.h"
#include "CircDLelement.h"
#include "/public/read.h"
#include <cstdlib>
#include <ctime>


using namespace bridges;
using namespace std;

//Initiative class
class HasInitiative {
    int speed;

    void set_speed(int newSpeed) {
        if (newSpeed >= 1 && newSpeed <= 40) {
            speed = newSpeed;
        } else {
            cout << "ERROR!\n";
            exit(1);
        }
    }
};
//D20 dice
void dice() {
    srand(time(0));
    int rando = (rand() % 20) + 1;
    //cout << rando << endl;
}

int main() {

    //Testing Dice
    /*  string in = "";
        cout << "INPUT: ";
        cin >> in;
        while (in != "Exit") {
        cout << dice() << endl;
        cin >> in;
        }

    */

    // create Bridges object
    Bridges bridges(1, "ForgotName",
                    "1362253532750");

    //----------------------------------WALL------------------------------------------
	//Loop to connnect Bridges and list for entities.
	CircDLelement<string> *turn = nullptr;
    vector<CircDLelement<string>*> storage;
    while (true) {
        string input = read("Enter Name (Type SNAPSHOT to view in bridges): ");
        if (input == "SNAPSHOT") break;
        CircDLelement<string> *entity = new CircDLelement<string>(to_string(dice()), input);
        storage.push_back(entity);

    }

    for (int i = 0; i < storage.size(); i++) {
        storage[i]->setNext(storage[(i + 1) % storage.size()]);
    }

    turn = storage[0];

    bridges.setDataStructure(turn);
    bridges.visualize();
    for (auto x : storage) {
        delete x;
    }

    return 0;
}
