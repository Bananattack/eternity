/* Copyright (c) 2010 Andrew Crowell and Jeffrey Tsang, all rights reserved.    *
 * See /doc/license.txt for details on how this source can be licensed for use. */

#pragma once
#include "event.h"
#include "block.h"
#include "condition.h"

namespace Eternity {
    class Node {
    private:
        Node parent;        /* the parent node that includes this instruction */
        Node f_branch;      /* the node to test next on condition false */
        Node t_branch;      /* the node to test next on condition true */
        Block instruction;  /* the block to execute, if terminal */
        Condition test;     /* the boolean condition to test, if internal */

        bool leaf;          /* whether the node is a terminal */
        bool active;        /* whether owning unit is currently executing this instruction, if terminal */
        bool satisfied;     /* whether the conditions are currently satisfied, if internal */

    public:
        bool isLeaf() {
            return leaf;
        }

        bool evaluateCondition();
    };
}

