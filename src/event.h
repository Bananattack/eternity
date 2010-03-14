#pragma once
#include "unit.h"

namespace Eternity {
    class Event {
    private:
        Unit invoker;
/* TODO: add (unit,effect on unit) lists and miscellany */
    }

/* wrapper for the event queue references returned by Battlefield.playwithEvent */
    class EventRef {
    private:
        multimap<int,Event>::iterator reference;
    public:
        EventRef(multimap<int,Event>::iterator input) {
            reference = input;
        }

        friend class Battlefield;
    }
}

