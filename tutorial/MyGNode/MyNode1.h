/***************************

@File: CMyGraphNode1.h
@Time: 2021/5/4 3:57 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYNODE1_H
#define DAOGRAPH_MYNODE1_H

#include "DaoGraph.h"

class MyNode1 : public DaoGraph::GNode {
public:
    CStatus run() override {
        CStatus status;
        DaoGraph::DAOGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        DAOGRAPH_SLEEP_SECOND(1)
        return status;
    }
};

#endif //DAOGRAPH_MYNODE1_H
