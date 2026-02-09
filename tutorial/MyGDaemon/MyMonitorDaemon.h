/***************************

@File: MyMonitorDaemon.h
@Time: 2022/2/3 12:08 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYMONITORDAEMON_H
#define DAOGRAPH_MYMONITORDAEMON_H

#include "DaoGraph.h"

class MyMonitorDaemon : public DaoGraph::GDaemon {
public:
    CVoid daemonTask(DaoGraph::GDaemonParamPtr param) override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyMonitorDaemon] this monitor is still running, monitor span is [%ld] ms...",
                            getInterval());
    }
};

#endif //DAOGRAPH_MYMONITORDAEMON_H
