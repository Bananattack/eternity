/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

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

