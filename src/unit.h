/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "battlefield.h"
using std:string;
using std::map;

namespace Eternity {
/* base class for game units */
    class Unit {
    private:
        int unit_id;
        string name;
        Location position;
        Node decision_root;
        map<String,Block> instructions;

        map<int,int> mem_int;
        map<int,double> mem_real;
        map<int,string*> mem_str;
        map<int,Unit*> mem_unit;
        map<int,Location> mem_loc;

        [enum] unit_type;
    public:
        bool modifyDecisions();
        bool modifyInstructions();

        int getID() const;
        string getName() const;
        Location getPosition() const;

        bool setID();
        bool setName(String);

        int queryMemInt(int) const;
        double queryMemReal(int) const;
        string* queryMemString(int) const;
        Unit* queryMemUnit(int) const;
        Location queryMemLocation(int) const;

        bool cacheState();
        bool cacheStatus();

        bool elapseCallback(int); /* unit will act by calling Battlefield.scheduleEvent from this */

        bool applyState();
        bool applyStatus();
    };

/* base class for unit decision-tree nodes */
    class Node {
    private:
        /**/ unit_set;      /* the set of units filtered through to this node */
        bool satisfied;     /* whether the conditions are currently satisfied */
    public:
        virtual bool evaluate() = 0;
    };

/* class for unit decision-tree normal internal nodes */
    class Node_Internal:Node {
    private:
        Node f_branch;      /* the node to test next on condition true */
        Node t_branch;      /* the node to test next on condition false */
        Condition test;     /* the boolean logic statement to test */
    public:
        bool evaluate();
    }

/* class for unit decision-tree filtering internal nodes */
    class Node_Filter:Node {
    private:
        Node f_branch;      /* the node to test next on unit-set nonempty */
        Node t_branch;      /* the node to test next on unit-set empty */
        Filter test;        /* the boolean logic filter to use */
    public:
        bool evaluate();
    }

/* class for unit decision-tree normal leaves */
    class Node_Terminal:Node {
    private:
        Block instruction;      /* the instruction to execute from this node */
        Block_Timer interrupt;  /* the function to calculate interrupt waiting time */
    public:
        bool evaluate();
    }

/* class for unit decision-tree sorting leaves */
    class Node_Sorter:Node {
    private:
        Block instruction;      /* the instruction to execute from this node */
        Block_Timer interrupt;  /* the function to calculate interrupt waiting time */
        Block_Sorter ordering;  /* the ordering function to pick a unit to target */
    public:
        bool evaluate();
    }

/* a unit instruction block */
    class Block {
/* TODO */
    };

/* a special-purpose unit instruction block for computing interrupt wait timers */
    class Block_Timer:Block {
    };

/* a special-purpose unit instruction block for sorting filtered unit for targetting */
    class Block_Sorter:Block {
    };

/* a special-purpose unit instruction block for handling action interrupt events */
    class Block_Interrupt:Block {
    };

/* a boolean-logic condition statement */
    class Condition {
/* TODO */
    };

/* a boolean unit-selection filter */
    class Filter:Condition {
/* TODO */
    }
}

