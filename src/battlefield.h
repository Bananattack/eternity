/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "order.h"
#include "unit.h"
using std::string;
using std::list;
using std::map;
using std::multimap;

namespace Eternity {
/* the base class for the entire battlefield */
    class Battlefield {
    private:
        bool paused;        /* whether game is in decision phase */
        bool issue_pause;   /* will the game pause after current issue */
        int game_tick;      /* game tick currently calculated */

        list<Order*> order_queue;           /* list of all orders pending */
        multimap<int,Event*> event_queue;   /* (tick,event) multimap queue */
        list<Continuous*> event_continuous; /* list of all continuous events */
        map<int,Unit*> unit_list;           /* (ID,unit) list of all units on map */

        void elapseTick();
        bool cacheStatus();
        bool applyStatus();
        bool issueOrder(Order*);
        void handleEvent(const Event*);
        bool handleContinuous(const Continuous*);
    public:
        Battlefield();
        ~Battlefield();

        int elapseGameTime(int);

        bool toggleIssuePause();
        bool playerInterrupt();
        bool requestInterrupt(const Unit&, string);

        int insertOrder(const Order*);
        bool deleteOrder(int);

        EventRef scheduleEvent(const Event*, int);
        EventRef delayEvent(EventRef, int);
        bool deleteEvent(EventRef);

        ContRef registerContinuous(const Continuous*);
        bool deleteContinuous(ContRef);

        void registerUnit(const Unit*);
        bool renameUnit(int, string);
        bool deleteUnit(int);
        Unit* getUnit(int);
        map<int,Unit*>::iterator getUnitList();
        /**/ getDirtyUnits();
    };

/* a unit map location wrapper */
    class Location {
    private:
        int x;
        int y;
        int height;
        int map;
    public:
        Location(int, int, int, int);

        int getX() const;
        int getY() const;
        int getHeight() const;
        int getMap() const;

        bool stepX(bool);
        bool stepY(bool);

        bool setX(int);
        bool setY(int);
        bool setHeight(int);
        bool setMap(int);

        int lineDistanceTo(Location) const;
    }

/* a game-play event */
    class Event {
    private:
        Unit invoker;
        map<Filter,list<Effect>> effect_list;
    public:
        virtual bool occur();
        virtual bool isContinuous();
/* TODO */
    };

/* a continuous game-play event */
    class Continuous:Event {
        Condition ending;
    public:
        bool occur();
        bool isContinuous();
    };

/* a game-play effect */
    class Effect {
    public:
        bool affect(Unit*);
/* TODO */
    };

/* wrapper for event queue references */
    class EventRef {
    private:
        multimap<int,Event*>::iterator reference;
    public:
        EventRef(multimap<int,Event*>::iterator);

        int getTick() const;
        Event* getEvent() const;
        multimap<int,Event*>::iterator getIterator() const;
    };

/* wrapper for continuous event references */
    class ContRef {
    private:
        list<Continuous*>::iterator reference;
    public:
        ContRef(list<Continuous*>::iterator);

        Continuous* getEvent() const;
        list<Continuous*>::iterator getIterator() const;
    };
}

