/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#ifndef UNIT_H
#define UNIT_H

#include "unit.h"
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
        Battlefield& field;
        bool dirty;
        const map<int,Unit*>* unit_set_base;
        const set<int>* dunit_set_base;
        Node decision_root;
        map<string,Block> instructions;
        list<Buff*> buff_list;

        map<int,bool> mem_bool;
        map<int,int> mem_int;
        map<int,double> mem_real;
        map<int,string*> mem_str;
        map<int,Unit*> mem_unit;
        map<int,Location> mem_loc;
    public:
        bool modifyDecisions();
        bool modifyInstructions();

        int getID() const;
        string getName() const;
        Location getPosition() const;
        bool getDirty() const;
        const map<int,Unit*>* getUnitList() const;
        const set<int>* getDirtyUnitList() const;

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

        bool elapseCallback(map<int,Unit*>::const_iterator, map<int,Unit*>::const_iterator, set<int>::const_iterator, set<int>::const_iterator); /* unit will act by calling Battlefield.scheduleEvent from this */

        bool applyState();
        bool applyStatus();
    };

/* base class for unit decision-tree nodes */
    class Node {
    protected:
        bool satisfied;                     /* whether the conditions are currently satisfied */
        bool modified;                      /* whether this node has been modified this tick */
        Node* last_filter;                  /* the last filtering node passed through this path */
    public:
        virtual bool evaluate(Unit&, Node*, const map<int,Unit*>*, const set<int>*) = 0;
    };

/* class for unit decision-tree normal internal nodes */
    class Node_Internal:public Node {
    private:
        Node* f_branch;                     /* the node to test next on condition true */
        Node* t_branch;                     /* the node to test next on condition false */
        Condition* test;                    /* the boolean logic statement to test */
        const map<int,Unit*>* unit_set_b;   /* the set of units filtered through to this node */
        const set<int>* dunit_set_b;        /* the set of dirty units filtered through to this node */
    public:
        Node* getFBranch() const;
        Node* getTBranch() const;
        Condition* getTest() const;

        bool setFBranch(const Node*);
        bool setTBranch(const Node*);
        bool setCondition(const Condition*);

        bool evaluate(Unit&, Node*, const map<int,Unit*>*, const set<int>*);
    }

/* class for unit decision-tree filtering internal nodes */
    class Node_Filter:public Node {
    private:
        Node* f_branch;                     /* the node to test next on unit-set nonempty */
        Node* t_branch;                     /* the node to test next on unit-set empty */
        Filter* test;                       /* the boolean logic filter to use */
        map<int,Unit*> unit_set;            /* the set of units filtered past this node */
        set<int> dirty_unit_set;            /* the set of dirty units filtered past this node */
//        bool dirty;                         /* whether the variables named in the filter have changed */
    public:
        Node* getFBranch() const;
        Node* getTBranch() const;
        Filter* getTest() const;

        bool setFBranch(const Node*);
        bool setTBranch(const Node*);
        bool setFilter(const Filter*);

        bool evaluate(Unit&, Node*, const map<int,Unit*>*, const set<int>*);
    }

/* class for unit decision-tree normal leaves */
    class Node_Terminal:public Node {
    private:
        Block* instruction;                 /* the instruction to execute from this node */
        Block_Timer* interrupt;             /* the function to calculate interrupt waiting time */
    public:
        Block* getInstruction() const;
        Block_Timer* getInterrupt() const;

        bool setInstruction(Block*);
        bool setInterrupt(Block_Timer*);

        bool evaluate(Unit&, Node*, const map<int,Unit*>*, const set<int>*);
    }

/* class for unit decision-tree sorting leaves */
    class Node_Sorter:public Node {
    private:
        Block* instruction;                 /* the instruction to execute from this node */
        Block_Timer* interrupt;             /* the function to calculate interrupt waiting time */
        Comparer* ordering;                 /* the ordering function to pick a unit to target */
        const map<int,Unit*>* unit_set_b;   /* the set of units filtered through to pick from */
//        bool dirty;                         /* whether the variables named in the ordering have changed */
    public:
        Block* getInstruction() const;
        Block_Timer* getInterrupt() const;
        Comparer* getOrdering() const;

        bool setInstruction(Block*);
        bool setInterrupt(Block_Timer*);
        bool setOrdering(Comparer*);

        bool evaluate(Unit&, Node*, const map<int,Unit*>*, const set<int>*);
    }

/* a unit instruction block */
    class Block {
    protected:
/* TODO */
    public:
    };

/* a special-purpose unit instruction block for computing interrupt wait timers */
    class Block_Timer:public Block {
    };

/* a special-purpose unit instruction block for handling action interrupt events */
    class Block_Interrupt:public Block {
    };

/* a boolean-logic condition statement */
    class Condition {
    protected:
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

/* an effect-list tied to a unit */
    class Buff {
    }
}

#endif /*UNIT_H*/

