/***************************

@File: MyNode2.h
@Time: 2021/5/4 4:09 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYNODE2_H
#define DAOGRAPH_MYNODE2_H

#include "DaoGraph.h"

class MyNode2 : public DaoGraph::GNode {
public:
    // 执行过程中，init 和 destroy 方法，单次执行，可选择性实现
    // run 方法可以多次执行，且必须实现
    CStatus init() override {
        CStatus status;
        DaoGraph::DAOGRAPH_ECHO("[INIT] [%s], enter MyNode2 init function.", this->getName().c_str());
        return status;
    }

    CStatus run() override {
        CStatus status;
        DaoGraph::DAOGRAPH_ECHO("[%s], enter MyNode2 run function. Sleep for 2 second ... ", this->getName().c_str());
        DAOGRAPH_SLEEP_SECOND(2)
        return status;
    }

    CStatus destroy() override {
        CStatus status;
        DaoGraph::DAOGRAPH_ECHO("[DESTROY] [%s], enter MyNode2 destroy function.", this->getName().c_str());
        return status;
    }
};

#endif //DAOGRAPH_MYNODE2_H
