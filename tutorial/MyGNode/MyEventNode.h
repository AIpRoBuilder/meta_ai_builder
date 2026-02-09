/***************************

@File: MyEventNode.h
@Time: 2023/1/20 18:31
@Desc: 
***************************/

#ifndef DAOGRAPH_MYEVENTNODE_H
#define DAOGRAPH_MYEVENTNODE_H

#include "DaoGraph.h"

class MyEventNode : public DaoGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        DaoGraph::DAOGRAPH_ECHO("[%s], before event notify", this->getName().c_str());
        /**
         * 模拟在这里，触发一个 trigger信息，同名的事件被异步执行
         * 从打印结果可以看出，after event send 这条信息，提前执行
         * 执行的时候，和pipeline公用同一个线程池资源
         */
        notify("my-print-event", GEventType::SYNC);

        DaoGraph::DAOGRAPH_ECHO("[%s], after event notify", this->getName().c_str());
        return status;
    }
};

#endif //DAOGRAPH_MYEVENTNODE_H
