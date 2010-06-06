/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "order.h"
#include "unit.h"
using std::string;
using std::list;
using std::map;
using std::multimap;

namespace Eternity {
    class Battlefield;
    class Location;
    class Event;
    class Continuous;
    class Effect;
    class EventRef;
    class ContRef;

/* the base class for the entire battlefield */
    class Battlefield {
    private:
        bool paused;                        /* whether game is in decision phase */
        bool issue_pause;                   /* will the game pause after current issue */
        int game_tick;                      /* game tick currently calculated */

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
        bool playerInterrupt();                         /* player interrupts execution handler */
        bool requestInterrupt(const Unit&, string);     /* unit interrupt request handler */

        int insertOrder(const Order*);
        bool deleteOrder(int);

        EventRef scheduleEvent(const Event*, int);      /* schedule an Event specified ticks later */
        EventRef delayEvent(EventRef, int);             /* delay an already scheduled Event */
        bool deleteEvent(EventRef);                     /* delete an already scheduled Event */

        ContRef registerContinuous(const Continuous*);  /* register a Continuous event */
        bool deleteContinuous(ContRef);                 /* de-register a Continuous event */

        void registerUnit(const Unit*);                 /* register a Unit onto the map */
        bool renameUnit(int, string);                   /* wrapper for renaming a specific Unit */
        bool deleteUnit(int);                           /* de-register a Unit off the map */
        Unit* getUnit(int);                             /* retrieve a Unit by unit_ID */
        map<int,Unit*>::iterator getUnitList();         /* retrieve the full list of map Units */
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
    class Continuous:public Event {
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

#endif /*BATTLEFIELD_H*/

