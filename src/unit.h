#pragma once
#include "node.h"
using std::map;

namespace Eternity {
    class Unit {
    private:
        Node decision_root;
        map<Block> instructions;
/* TODO: add a hashtable or similar for "state variables being listened to by instruction conditions"
 * differentiate between a battlefield state variable and a unit-condition variable (property of a unit/tile satisfying some condition)
 * fix unit properties and put them here */
    public:
        bool modifyInstructions();

        bool updateState(int, int);
        bool updateState(int, double);

        int queryState(int); /* TODO: add question passing convention */
        bool listenCallback(int); /* unit will act by calling Battlefield.scheduleEvent from this, TODO: add and fix callback argument passing convention */
    }
}

