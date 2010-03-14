#pragma once
#include "event.h"

namespace Eternity {
    class Instruction {
    private:
        Instruction parent;     /* the parent block that includes this instruction */
        bool type_simple;       /* whether this instruction invokes actual actions or nests further instructions */

        bool satisfied;         /* whether the conditions are currently satisfied */
        bool active;            /* whether owning unit is currently executing this instruction */
        Instruction exec_child; /* if compound and active, which of the child instructions is being executed */

/* TODO: add condition block and action block */
    public:
        bool isActionSimple() {
            return type_simple;
        }

        bool evaluateCondition();
    };

    class Condition {
/* TODO */
    }
}

namespace {
/* file-scoped class for an game-level atomic action */
    class Action {
/* TODO */
    }
}

