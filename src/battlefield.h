/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "event.h"
#include "instruction.h"
#include "order.h"
#include "unit.h"
using std::list;
using std::multimap;

namespace Eternity {
    class Battlefield {
    private:
        bool paused;        /* whether game in decision phase */
        bool issue_pause;   /* will the game pause after current issue */
        int game_tick;      /* game tick currently calculated */
        int drawn_tick;     /* game tick actually rendered */

        list<Order> order_queue;

        int next_event_tick;                /* lowest queued event tick number */
        multimap<int,Event> event_queue;    /* (tick,event) multimap queue */
        list<Event> event_continuous;       /* list of all continuous events */

//        multimap<something> callback_map;

        bool issueOrder(Order);
        void handleEvent(Event);
    public:
        Battlefield();
        ~Battlefield();

        int elapseGameTime(int);

        bool toggleIssuePause() {
            return (issue_pause = !issue_pause);
        }
        bool playerInterrupt();
        bool requestInterrupt(const Unit); /* TODO: add reason for interrupt handling */

        int insertOrder(const Order);
        bool deleteOrder(int);

        EventRef scheduleEvent(const Event);
        EventRef delayEvent(EventRef, int);
        bool deleteEvent(EventRef);

        registerCallback(const Unit, int); /* a unit calls this to register a state variable callback listener, TODO: add condition passing convention */
        deregisterCallback(const Unit, int);
    }
}

