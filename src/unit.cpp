/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#include "eternity.h"
#include "unit.h"

namespace Eternity {
/* Unit class methods */
    int Unit::getID() const {
        return unit_id;
    }

    string Unit::getName() const {
        return name;
    }

    Location Unit::getPosition() const {
        return position;
    }

    bool Unit::getDirty() const {
        return dirty;
    }

    const map<int,Unit*>* Unit::getUnitList() const {
        return unit_set_base;
    }

    const set<int>* Unit::getDirtyUnitList() const {
        return dunit_set_base;
    }

    bool Unit::setID(int new_id) {
        unit_id = new_id;
        return true;
    }

    bool Unit::setName(string new_name) {
        name = new_name;
        return true;
    }

/* Node_Internal class methods */
    Node* Node_Internal::getFBranch() const {
        return f_branch;
    }

    Node* Node_Internal::getTBranch() const {
        return t_branch;
    }

    Condition* Node_Internal::getTest() const {
        return test;
    }

    bool Node_Internal::setFBranch(const Node* new_branch) {
        f_branch = new_branch;
        modified = true;
        return true;
    }

    bool Node_Internal::setTBranch(const Node* new_branch) {
        t_branch = new_branch;
        modified = true;
        return true;
    }

    bool Node_Internal::setCondition(const Condition* new_test) {
        test = new_test;
        modified = true;
        return true;
    }

    bool Node_Internal::evaluate(Unit& caller, Node* passing_filter, const map<int,Unit*>* punit_set, const set<int>* pdunit_set) {
        unit_set_b = punit_set;
        dunit_set_b = pdunit_set;
        last_filter = passing_filter;
/* TODO: check also for dirty */
        if (modified ? (modified = false, satisfied = test->evaluate()) : satisfied) {
            return t_branch->evaluate(caller, last_filter, unit_set_b, dunit_set_b);
        } else {
            return f_branch->evaluate(caller, last_filter, unit_set_b, dunit_set_b);
        }
    }

/* Node_Filter class methods */
    Node* Node_Filter::getFBranch() const {
        return f_branch;
    }

    Node* Node_Filter::getTBranch() const {
        return t_branch;
    }

    Filter* Node_Filter::getTest() const {
        return test;
    }

    bool Node_Filter::setFBranch(const Node* new_branch) {
        f_branch = new_branch;
        modified = true;
        return true;
    }

    bool Node_Filter::setTBranch(const Node* new_branch) {
        t_branch = new_branch;
        modified = true;
        return true;
    }

    bool Node_Filter::setCondition(const Filter* new_test) {
        test = new_test;
        modified = true;
        return true;
    }

    bool Node_Filter::evaluate(Unit& caller, Node* passing_filter, const map<int,Unit*>* punit_set, const set<int>* pdunit_set) {
        int t_id;
        set<int> tunit_set;
        map<int,Unit*>::iterator unit_cur, unit_tmp, unit_max;
        map<int,Unit*>::const_iterator punit_cur, punit_max;
        set<int>::iterator dunit_cur, dunit_tmp, dunit_max, tunit_cur, tunit_max;
        dunit_set.clear();
        dunit_cur = dunit_set.begin();
        if (test->checkDirty() || (passing_filter != last_filter) || modified) {
            /* cache unusable, compute the filter-set from scratch */
            last_filter = passing_filter;
            modified = false;
            /* initialize dunit_set to unit_set, then clear unit_set */
            for (unit_cur = unit_set.begin(), unit_max = unit_set.end(); unit_cur != unit_max; unit_cur++) {
                dunit_cur = dunit_set.insert(dunit_cur, unit_cur->first);
            }
            unit_set.clear();
            /* evaluate every unit in unit_set_p for inclusion into unit_set and tunit_set */
            dunit_cur = tunit_set.begin()
            for (punit_cur = punit_set->begin(), punit_max = punit_set->end(); punit_cur != punit_max; punit_cur++) {
                if (test->evaluate(punit_cur->second)) {
                    unit_cur = unit_set.insert(unit_cur, *punit_cur);
                    dunit_cur = tunit_set.insert(dunit_cur, punit_cur->first);
                }
            }
            /* a unit past this filter is dirty if it (was in, now out), or vv, or if it was marked as dirty and (was in, now in)
             * (in tunit_set <==> now in), (in dunit_set <==> was in), (in pdunit_set <==> was dirty) */
            dunit_cur = dunit_set.begin(); dunit_max = dunit_set.end();
            punit_cur = pdunit_set->begin(); punit_max = pdunit_set->end();
            for (tunit_cur = tunit_set.begin(), tunit_max = tunit_set.end(); tunit_cur != tunit_max; tunit_cur++) {
                tid = *tunit_cur;
                if (dunit_cur != dunit_max) {
                    while ((*dunit_cur < tid) && (++dunit_cur != dunit_max));
                    if ((dunit_cur != dunit_max) && (*dunit_cur == tid)) {
                        /* tunit_cur in dunit_set */
                        if (punit_cur != punit_max) {
                            while ((*punit_cur < tid) && (++punit_cur != punit_max));
                            if ((punit_cur == punit_max) || *punit_cur > tid) {
                                /* tunit_cur not in pdunit_set: remove from dunit_set */
                                dunit_tmp = dunit_cur;
                                dunit_cur++;
                                dunit_set.erase(dunit_tmp);
                            }
                        } else {
                            /* pdunit_set iterated through, equal to above block */
                            dunit_tmp = dunit_cur;
                            dunit_cur++;
                            dunit_set.erase(dunit_tmp);
                        }
                    } else {
                        /* tunit_cur not in dunit_set: add to dunit_set */
                        dunit_tmp = dunit_cur;
                        dunit_set.insert(--dunit_tmp, tid);
                    }
                } else {
                    /* dunit_set iterated through, equal to above block */
                    dunit_tmp = dunit_cur;
                    dunit_set.insert(--dunit_tmp, tid);
                }
            }
        } else {
            /* cache valid, re-check all units marked dirty
             * a unit past this filter is dirty if it was marked dirty, and !(was out, now out) */
            unit_cur = unit_set.begin(); unit_max = unit_set.end();
            for (punit_cur = pdunit_set->begin(), punit_max = pdunit_set->end(); punit_cur != punit_max; punit_cur++) {
                tid = *punit_cur;
                if (test->evaluate(punit_cur->second)) {
                    if (unit_cur != unit_max) {
                        while ((*unit_cur < tid) && (++unit_cur != unit_max));
                        if ((unit_cur == unit_max) || (*unit_cur > tid)) {
                            /* punit_cur not in unit_set: add to unit_set - assumed rare event */
                            unit_cur = unit_set.insert(--unit_cur, punit_set->find(tid));
                        }
                    } else {
                        /* unit_set iterated through, equal to above block */
                        unit_cur = unit_set.insert(--unit_cur, punit_set->find(tid));
                    }
                    dunit_cur = dunit_set.insert(dunit_cur, tid);
                } else {
                    if (unit_cur != unit_max) {
                        while ((*unit_cur < tid) && (++unit_cur != unit_max));
                        if ((unit_cur != unit_max) && (*unit_cur == tid)) {
                            /* punit_cur in unit_set: remove from unit_set and add to dunit_set */
                            unit_tmp = unit_cur;
                            unit_cur++;
                            unit_set.erase(unit_tmp);
                            dunit_cur = dunit_set.insert(dunit_cur, tid);
                        }
                    }
                }
            }
        }
        if (satisfied = !unit_set.empty()) {
            return t_branch->evaluate(caller, this, &unit_set, &dunit_set);
        } else {
            return f_branch->evaluate(caller, NULL, caller.getUnitList(), caller.getDirtyUnitList());
        }
    }

/* Node_Terminal class methods */
    Block* Node_Terminal::getInstruction() const {
        return instruction;
    }

    Block_Timer* Node_Terminal::getInterrupt() const {
        return interrupt;
    }

    bool Node_Terminal::setInstruction(const Block* new_block) {
        instruction = new_block;
        modified = true;
        return true;
    }

    bool Node_Terminal::setInterrupt(const Block_Timer* new_block) {
        interrupt = new_block;
        modified = true;
        return true;
    }

    bool Node_Terminal::evaluate(Unit& caller, Node* passing_filter, const map<int,Unit*>* punit_set, const set<int>* pdunit_set) {
/* TODO */
    }

/* Node_Sorter class methods */
    Block* Node_Sorter::getInstruction() const {
        return instruction;
    }

    Block_Timer* Node_Sorter::getInterrupt() const {
        return interrupt;
    }

    Comparer* Node_Sorter::getOrdering() const {
        return ordering;
    }

    bool Node_Sorter::setInstruction(Block* new_block) {
        instruction = new_block;
        modified = true;
        return true;
    }

    bool Node_Sorter::setInterrupt(Block_Timer* new_block) {
        interrupt = new_block;
        modified = true;
        return true;
    }

    bool Node_Sorter::setOrdering(Comparer* new_ordering) {
        ordering = new_ordering;
        modified = true;
        return true;
    }

    bool Node_Sorter::evaluate(Unit& caller, Node* passing_filter, const map<int,Unit*>* punit_set, const set<int>* pdunit_set) {
/* TODO */
    }    
}

