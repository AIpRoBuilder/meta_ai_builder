/***************************

@File: MyPrintEvent.h
@Time: 2023/1/20 17:57
@Desc: 
***************************/

#ifndef DAOGRAPH_MYPRINTEVENT_H
#define DAOGRAPH_MYPRINTEVENT_H

#include "DaoGraph.h"
#include "../MyParams/MyParam.h"

class MyPrintEvent : public DaoGraph::GEvent {
public:
    /**
     * 在本事件被触发的时候，会执行到这里
     * @return
     */
    CVoid trigger(DaoGraph::GEventParamPtr param) override {
        DAOGRAPH_SLEEP_MILLISECOND(100)    // 留100ms的耗时，便于看出来同步/异步触发机制

        /**
         * 这里是为了表示，event内部也可以获取pipeline 内部的数据
         * 在并发的情况下使用，需要考虑数据同步和互斥的问题
         */
        auto myParam = DAOGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        DaoGraph::DAOGRAPH_ECHO("----> trigger [%d] times, iValue = [%d]", times_++, myParam->iValue);
    }

private:
    int times_ = 0;
};

#endif //DAOGRAPH_MYPRINTEVENT_H
