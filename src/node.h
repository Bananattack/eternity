/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "event.h"

namespace Eternity {
    class Node {
    private:
        Node parent;        /* the parent node that includes this instruction */
        Node f_branch;      /* the node to test next on condition false */
        Node t_branch;      /* the node to test next on condition true */
        Block instruction;  /* the block to execute, if terminal */

        bool leaf;          /* whether the node is a terminal */
        bool satisfied;     /* whether the conditions are currently satisfied */
        bool active;        /* whether owning unit is currently executing this instruction */

/* TODO: add condition block and action block */
    public:
        bool isLeaf() {
            return leaf;
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

