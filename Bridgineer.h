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

class LL {

  public:
    int data;
    LL* prev;
    LL* next;

    LL(int x) {
        data = x;
    }

};

class HasInitiative {
    int speed;

    void set_speed(int newSpeed) {
        if (newSpeed >= 1 || newSpeed <= 40) {
            speed = newSpeed;
        } else {
            cout << "ERROR!\n";
            exit(1);
        }
    }
};

void dice() {
    srand(time(0));
    int rando = (rand() % 20) + 1;
    cout << rando << endl;
}

int main() {

    dice();

    // create Bridges object, set credentials
    Bridges bridges(1, "ForgotName",
                    "1362253532750");

    // set title, description
    bridges.setTitle("A Simple  Binary Search Tree Example");
    bridges.setDescription("This example illustrates a binary search tree built using BRIDGES");

    // we will create the tree manually in this example

    CircDLelement<string> *first = new CircDLelement<string>("#1", "#TEMP1");
    CircDLelement<string> *secon = new CircDLelement<string>("#2", "#TEMP2");
    CircDLelement<string> *third = new CircDLelement<string>("#3", "#TEMP3");

    first->setNext(secon);
    secon->setNext(third);
    //secon->setPrev(first);
    third->setNext(first);
    //third->setPrev(secon);

    //first->setPrev(third);


    bridges.setDataStructure(first);
    bridges.visualize();
    delete first;
    delete secon;
    delete third;
    return 0;
}

