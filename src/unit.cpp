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

    bool Unit::setID(int new_id) {
        unit_id = new_id;
        return true;
    }

    bool Unit::setName(string new_name) {
        name = new_name;
        return true;
    }
}

