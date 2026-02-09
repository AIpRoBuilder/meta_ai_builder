/***************************

@File: MyTraceAspect.h
@Time: 2021/9/29 10:51 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYTRACEASPECT_H
#define DAOGRAPH_MYTRACEASPECT_H

#include "DaoGraph.h"

class MyTraceAspect : public DaoGraph::GAspect {
public:
    /**
     * 实现几个切面函数逻辑，模拟在对应执行的位置，打印trace信息
     */
    CStatus beginInit() override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] init begin ...", this->getName().c_str());
        return CStatus();
    }

    CVoid finishInit(const CStatus& curStatus) override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] init finished, error code is [%d] ...", this->getName().c_str(),
                            curStatus.getCode());
    }

    CStatus beginRun() override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] run begin ...", this->getName().c_str());
        return CStatus();
    }

    CVoid finishRun(const CStatus& curStatus) override {
        if (!curStatus.isOK()) {
            DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, status is ok ...", this->getName().c_str());
        } else {
            DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] run finished, error code is [%d] ...", this->getName().c_str(),
                                curStatus.getCode());
        }
    }

    CStatus beginDestroy() override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] destroy begin ...", this->getName().c_str());
        return CStatus();
    }

    CVoid finishDestroy(const CStatus& curStatus) override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTraceAspect] [%s] destroy finished, error code is [%d] ...", this->getName().c_str(),
                            curStatus.getCode());
    }
};

#endif //DAOGRAPH_MYTRACEASPECT_H
