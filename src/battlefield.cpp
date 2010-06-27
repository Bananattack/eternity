/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#include "eternity.hpp"
#include "battlefield.hpp"

namespace Eternity {
/* Battlefield class methods */
    void Battlefield::elapseTick() {
        multimap<int,Event*>::iterator qevent_cur, qevent_max;
        list<Continuous*>::iterator cevent_cur, cevent_max;
        map<int,Unit*>::const_iterator ulist_max, ulist_cur;
        /* cache all unit/effect statuses */
        ulist_max = unit_list.end();
        for (unit_cur = unit_list.begin(); ulist_cur != ulist_max; ulist_cur++) {
            ulist_cur->cacheStatus();
        }
        cacheStatus();
        /* handle one-shot events on this tick */
        qevent_max = event_queue.upper_bound(++game_tick);
        while ((qevent_cur = event_queue.begin()) != qevent_max) {
            handleEvent(qevent_cur->second);
            event_queue.erase(qevent_cur);
        }
        /* handle continuous events */
        for (cevent_cur = event_continuous.begin(), cevent_max = event_continuous.end(); cevent_cur != cevent_max; cevent_cur++) {
            if (handleContinuous(cevent_cur->second)) {
                event_continuous.erase(cevent_cur);
            }
        }
        /* apply changes to all unit/effect statuses */
        for (unit_cur = unit_list.begin(); ulist_cur != ulist_max; ulist_cur++) {
            ulist_cur->applyStatus();
        }
        applyStatus();
        /* cache all unit internal states */
        for (unit_cur = unit_list.begin(); ulist_cur != ulist_max; ulist_cur++) {
            ulist_cur->cacheState();
        }
/*TODO: issue order if any */
        /* cycle through all units and have them act */
        for (unit_cur = unit_list.begin(); ulist_cur != ulist_max; ulist_cur++) {
            ulist_cur->elapseCallback() /*API may change*/
        }
        /* apply changes to all unit internal states */
        for (unit_cur = unit_list.begin(); ulist_cur != ulist_max; ulist_cur++) {
            ulist_cur->applyState();
        }
    }

    bool Battlefield::issueOrder(Order order) {
/* TODO */
        if (issue_pause) {
            paused = true;
        }
    }

    void Battlefield::handleEvent(const Event event) {
/* TODO */
    }

    bool Battlefield::handleContinuous(const Continuous event) {
/* TODO */
    }

    int Battlefield::elapseGameTime(int ticks) {
        while (--ticks >= 0) {
            elapseTick();
        }
    }

    bool Battlefield::toggleIssuePause() {
        return (issue_pause = !issue_pause);
    }

    EventRef Battlefield::scheduleEvent(const Event* event, int delay) {
        return EventRef(event_queue.insert(event_queue.begin(), pair<int,Event*>(game_tick + delay, event)));
    }

    EventRef Battlefield::delayEvent(EventRef ref, int delay) {
        EventRef ret = EventRef(event_queue.insert(ref.getIterator(), pair<int,Event*>(ref.getTick() + delay, ref.getEvent())));
        event_queue.erase(ref.getIterator());
        return ret;
    }

    bool Battlefield::deleteEvent(EventRef ref) {
        event_queue.erase(ref.getIterator());
        return true;
    }

    ContRef Battlefield::registerContinuous(const Continuous* event) {
        return ContRef(event_continuous.push_back(event));
    }

    bool Battlefield::deleteContinuous(ContRef ref) {
        event_continuous.erase(ref.getIterator());
        return true;
    }

    void Battlefield::registerUnit(const Unit* unit) {
        unit_list.insert(pair<int,Unit*>(unit->getID(), unit));
    }

    bool Battlefield::renameUnit(string old_name, string new_name) {
        map<int,Unit*>::iterator ref;
        Unit* unit;
        if ((ref = unit_list.find(old_name)) == unit_list.end()) {
            return false;
        }
        return ref->setName(new_name);
    }

    bool Battlefield::deleteUnit(int unit_id) {
        map<int,Unit*>::iterator ref;
        if ((ref = unit_list.find(unit_id)) == unit_list.end()) {
            return false;
        }
        unit_list.erase(ref);
        return true;
    }

    Unit* Battlefield::getUnit(int unit_id) const {
        map<int,Unit*>::iterator ref;
        if ((ref = unit_list.find(unit_id)) == unit_list.end()) {
            return NULL;
        }
        return ref->second;
    }

    const map<int,Unit*>* Battlefield::getUnitList() const {
        return &unit_list;
    }

    const set<int>* Battlefield::getDirtyUnitList() const {
        return &dunit_list;
    }

/* Location class methods */
    Location::Location(int d_x, int d_y, int d_height, int d_map) {
        x = d_x;
        y = d_y;
        height = d_height;
        map = d_map;
    }

    int Location::getX() const {
        return x;
    }

    int Location::getY() const {
        return y;
    }

    int Location::getHeight() const {
        return height;
    }

    int Location::getMap() const {
        return map;
    }

    bool Location::stepX(bool forward) {
        if (forward) {
            x++;
        } else {
            x--;
        }
        return true;
    }

    bool Location::stepY(bool forward) {
        if (forward) {
            y++;
        } else {
            y--;
        }
        return true;
    }

    bool Location::setX(int new_x) {
        x = new_x;
        return true;
    }

    bool Location::setY(int new_y) {
        y = new_y;
        return true;
    }

    bool Location::setHeight(int new_height) {
        height = new_height;
        return true;
    }

    bool Location::setMap(int new_map) {
        map = new_map;
        return true;
    }

    int Location::lineDistanceTo(Location dest) const {
/* TODO */
    }

/* Event class methods */
    virtual bool Event::isContinuous() {
        return false;
    }

/* Continuous class methods */
    bool Continuous::isContinuous() {
        return true;
    }

/* EventRef class methods */
    EventRef::EventRef(multimap<int,Event*>::iterator input) {
        reference = input;
    }

    int EventRef::getTick() const {
        return reference->first;
    }

    Event* EventRef::getEvent() const {
        return reference->second;
    }

    multimap<int,Event*>::iterator EventRef::getIterator() const {
        return reference;
    }

/* ContRef class methods */
    ContRef::ContRef(list<Continuous*>::iterator input) {
        reference = input;
    }

    Continuous* ContRef::getEvent() const {
        return *reference;
    }

    list<Continuous*>::iterator ContRef::getIterator() const {
        return reference;
    }
}

