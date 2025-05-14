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
Class HasInitiative {
  public:
    string name;
    int speed;
    int initiative;

  int get_speed() const{
  return speed;
  }
  
    void set_speed(int newSpeed) {
        if (newSpeed >= 1 && newSpeed <= 40) {
            speed = newSpeed;
        } else {
            cout << "ERROR!\n";
            exit(1);
        }
    }
    void set_name(string here) {
        name = here;
    }

    void print_Ini() const {
        cout << speed << "\n";
    }
    int get_Ini() const {
        return initiative;
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

    HasInitiative(string x, int s) {
        set_name(x);
        set_speed(s);
        roll_4();
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
    CircDLelement<int> *turn = nullptr;
    vector<CircDLelement<int>*> storage;
    while (true) {

        string input = read("Enter Name: ");
        if (input == "EXIT") break;

        int input2 = read("Enter Speed (between 1-40): ");
        if (input2 < 0) break;

        HasInitiative *entity = new HasInitiative(input, input2);
        storage.push_back(new CircDLelement<int>((entity->initiative), input));

        delete entity;

    }

//Sort by Initiative
    sort(storage.begin(), storage.end(), [](CircDLelement<int>* a, CircDLelement<int>* b) {
        return (a->getValue() > b->getValue());
    }
        );


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
