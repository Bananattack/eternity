/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "order.h"
#include "unit.h"
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

        list<Order> order_queue;            /* list of all orders pending */
        multimap<int,Event&> event_queue;   /* (tick,event) multimap queue */
        list<Event&> event_continuous;      /* list of all continuous events */
        map<String,Unit&> unit_list;        /* list of all units on map with mangled names */

        void elapseTick();
        bool issueOrder(Order&);
        void handleEvent(Event&);
        bool handleContinuous(Event&);
    public:
        Battlefield();
        ~Battlefield();

        int elapseGameTime(int);

        bool toggleIssuePause();
        bool playerInterrupt();
        bool requestInterrupt(const& Unit); /* TODO: add reason for interrupt handling */

        int insertOrder(const Order);
        bool deleteOrder(int);

        EventRef scheduleEvent(const Event&, int);
        EventRef delayEvent(EventRef, int);
        bool deleteEvent(EventRef);

        ContRef registerContinuous(const Event&);
        bool deleteContinuous(ContRef);

        void registerUnit(const Unit&, String);
        bool renameUnit(String, String);
        bool deleteUnit(String);
    };

/* a game-play event */
    class Event {
    private:
        Unit invoker;
/* TODO: add (unit,effect on unit) lists and miscellany */
    };

/* wrapper for event queue references */
    class EventRef {
    private:
        multimap<int,Event&>::iterator reference;
    public:
        EventRef(multimap<int,Event&>::iterator);

        int getTick() const;
        Event& getEvent() const;
        multimap<int,Event&>::iterator getIterator() const;
    };

/* wrapper for the continuous event references */
    class ContRef {
    private:
        list<Event&>::iterator reference;
    public:
        ContRef(list<Event&>::iterator);

        Event& getEvent() const;
        list<Event&>::iterator getIterator() const;
    };
}

