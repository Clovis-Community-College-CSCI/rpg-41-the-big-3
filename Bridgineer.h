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

class HasInitiative {
  public:
    int speed;
    int initiative;

    void set_speed(int newSpeed) {
        if (newSpeed >= 1 && newSpeed <= 40) {
            speed = newSpeed;
        } else {
            cout << "ERROR!\n";
            exit(1);
        }
    }
    void print_Ini() const {
        cout << speed << "\n";
    }


    int dice() {
        srand(time(0));
        int rando = (rand() % 20) + 1;
        cout << rando << endl;
        return rando;
    }

    void roll_4() {
        initiative = speed + dice();
    }

    HasInitiative(int s) {
        set_speed(s);

    }

};

int dice_test() {
    srand(time(0));
    int rando = (rand() % 20) + 1;
    cout << rando << endl;
    return rando;
}


int main() {

    // create Bridges object
    Bridges bridges(1, "ForgotName",
                    "1362253532750");

    //----------------------------------WALL------------------------------------------
    CircDLelement<string> *turn = nullptr;
    vector<CircDLelement<string>*> storage;
    while (true) {
        string input = read("Enter Name: ");
        if (input == "SNAPSHOT") break;
        CircDLelement<string> *entity = new CircDLelement<string>(to_string(dice_test()), input);
        storage.push_back(entity);

    }

    for (int i = 0; i < storage.size(); i++) {
        storage[i]->setNext(storage[(i + 1) % storage.size()]);
        storage[i]->setPrev(storage[(((i - 1) + storage.size()) % storage.size())]);
    }


    turn = storage[0];

    bridges.setDataStructure(turn);
    bridges.visualize();
    for (auto x : storage) {
        delete x;
    }

    return 0;
}
