/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#include "eternity.h"
#include "battlefield.h"

namespace Eternity {
    void Battlefield::elapseTick() {
        multimap<int,Event>::iterator qevent_max, qevent_cur;
        list<Event>::iterator cevent_max, cevent_cur;
/* handle one-shot events on this tick */
        qevent_max = event_queue.upper_bound(++game_tick);
        while ((qevent_cur = event_queue.begin()) != qevent_max) {
            handleEvent(qevent_cur->second);
            event_queue.erase(qevent_cur);
        }
/* handle continuous events */
        cevent_max = event_continuous.end();
        cevent_cur = event_continuous.begin();
        do {
            if (handleContinuous(cevent_cur->second)) {
                event_continuous.erase(cevent_cur);
            }
        } while (cevent_cur++ != cevent_max);
/* TODO: cycle through all units and re-test trees */
    }

    bool Battlefield::issueOrder(Order order) {
/* TODO */
    }

    void Battlefield::handleEvent(Event event) {
/* TODO */
    }

    bool Battlefield::handleContinuous(Event event) {
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

    EventRef Battlefield::scheduleEvent(const Event event, int delay) {
        return EventRef(event_queue.insert(event_queue.begin(), pair<int,Event>(game_tick + delay, event)));
    }

    EventRef Battlefield::delayEvent(EventRef ref, int delay) {
        EventRef ret = EventRef(event_queue.insert(ref.reference, pair<int,Event>(ref.getTick() + delay, ref.getEvent())));
        event_queue.erase(ref.reference);
        return ret;
    }

    bool Battlefield::deleteEvent(EventRef ref) {
        event_queue.erase(ref.reference);
        return true;
    }

    ContRef Battlefield::registerContinuous(const Event event) {
        return ContRef(event_continuous.push_back(event));
    }

    bool Battlefield::deleteContinuous(ContRef ref) {
        event_continuous.erase(ref.reference);
        return true;
    }
}

