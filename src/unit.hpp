/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#ifndef UNIT_H
#define UNIT_H

#include "battlefield.h"
using std:string;
using std::map;

namespace Eternity {
    class Unit;
    class Node;
    class Node_Internal;
    class Node_Filter;
    class Node_Terminal;
    class Node_Sorter;
    class Block;
    class Block_Timer;
    class Block_Interrupt;
    class Condition;
    class Filter;
    class Comparer;

/* base class for game units */
    class Unit {
    private:
        int unit_id;
        string name;
        Location position;
        Battlefield& field;
        bool dirty;
        map<int,Unit*>* dirty_unit_setp;
        Node decision_root;
        map<string,Block> instructions;

        map<int,bool> mem_bool;
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
        bool getDirty() const;
        map<int,Unit*>* getDirtyUnitList();

        bool setID();
        bool setName(String);

        bool queryMemBool(int) const;
        int queryMemInt(int) const;
        double queryMemReal(int) const;
        string* queryMemString(int) const;
        Unit* queryMemUnit(int) const;
        Location queryMemLocation(int) const;

        bool cacheState();
        bool cacheStatus();

        bool elapseCallback(map<int,Unit*>*); /* unit will act by calling Battlefield.scheduleEvent from this */

        bool applyState();
        bool applyStatus();
    };

/* base class for unit decision-tree nodes */
    class Node {
    private:
        bool satisfied;         /* whether the conditions are currently satisfied */
    public:
        virtual bool evaluate(Unit&) = 0;
    };

/* class for unit decision-tree normal internal nodes */
    class Node_Internal:public Node {
    private:
        Node f_branch;              /* the node to test next on condition true */
        Node t_branch;              /* the node to test next on condition false */
        Condition test;             /* the boolean logic statement to test */
        map<int,Unit*>* unit_setp;  /* the set of units filtered through to this node */
    public:
        bool evaluate(Unit&);
    }

/* class for unit decision-tree filtering internal nodes */
    class Node_Filter:public Node {
    private:
        Node f_branch;              /* the node to test next on unit-set nonempty */
        Node t_branch;              /* the node to test next on unit-set empty */
        Filter test;                /* the boolean logic filter to use */
        map<int,Unit*> unit_set;    /* the set of units filtered past this node */
        bool dirty;                 /* whether the variables named in the filter have changed */
    public:
        bool evaluate(Unit&);
    }

/* class for unit decision-tree normal leaves */
    class Node_Terminal:public Node {
    private:
        Block instruction;      /* the instruction to execute from this node */
        Block_Timer interrupt;  /* the function to calculate interrupt waiting time */
    public:
        bool evaluate(Unit&);
    }

/* class for unit decision-tree sorting leaves */
    class Node_Sorter:public Node {
    private:
        Block instruction;          /* the instruction to execute from this node */
        Block_Timer interrupt;      /* the function to calculate interrupt waiting time */
        Comparer ordering;          /* the ordering function to pick a unit to target */
        map<int,Unit*>* unit_setp;  /* the set of units filtered through to pick from */
        bool dirty;                 /* whether the variables named in the ordering have changed */
    public:
        bool evaluate(Unit&);
    }

/* a unit instruction block */
    class Block {
/* TODO */
    };

/* a special-purpose unit instruction block for computing interrupt wait timers */
    class Block_Timer:public Block {
    };

/* a special-purpose unit instruction block for sorting filtered unit for targetting */
    class Block_Sorter:public Block {
    };

/* a special-purpose unit instruction block for handling action interrupt events */
    class Block_Interrupt:public Block {
    };

/* a boolean-logic condition statement */
    class Condition {
/* TODO */
    };

/* a boolean unit-selection filter */
    class Filter:public Condition {
/* TODO */
    }

/* a boolean unit-comparison function */
    class Comparer:public Condition {
/* TODO */
    }
}

#endif /*UNIT_H*/

