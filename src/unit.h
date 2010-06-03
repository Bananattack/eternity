/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "battlefield.h"
using std::map;

namespace Eternity {
/* a game unit, which may be a 'normal' player-selectable or could be a doodad */
    class Unit {
    private:
        Node decision_root;
        map<String,Block> instructions;
        map<String,Variable> memory;

        [enum] unit_type;
    public:
        bool modifyDecisions();
        bool modifyInstructions();

        bool updateState(int, int);
        bool updateState(int, double);

        int queryState(int); /* TODO: add question passing convention */
        bool listenCallback(int); /* unit will act by calling Battlefield.scheduleEvent from this, TODO: add and fix callback argument passing convention */
    };

/* a unit decision-tree node */
    class Node {
    private:
        Node parent;        /* the parent node that includes this instruction */
        Node f_branch;      /* the node to test next on condition false */
        Node t_branch;      /* the node to test next on condition true */
        Block instruction;  /* the block to execute, if terminal */
        Condition test;     /* the boolean condition to test, if internal */

        bool leaf;          /* whether the node is a terminal */
        bool active;        /* whether owning unit is currently executing this instruction, if terminal */
        bool satisfied;     /* whether the conditions are currently satisfied, if internal */

    public:
        bool isLeaf() const;

        bool evaluateCondition();
    };

/* a unit instruction block */
    class Block {
/* TODO */
    };

/* a boolean-logic condition statement */
    class Condition {
/* TODO */
    };

/* a unit internal memory variable */
    class Variable {
    private:
        union {
            int val_int;
            double val_real;
            String& val_str;
            Unit& val_unit;
        };
        const Type type;
    public:
        static enum Type {
            INT,
            REAL,
            STRING,
            UNIT
        }

        void* getValue() const;

        bool setValue(int);
        bool setValue(double);
        bool setValue(String&);
        bool setValue(Unit&);
    };
}

