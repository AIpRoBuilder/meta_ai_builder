/***************************

@File: PywGDaemon.h
@Time: 2025/3/30 00:13
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGDAEMON_H
#define DAOGRAPH_PYWGDAEMON_H

#include "DaoGraph.h"

class PywGDaemon : public DaoGraph::GDaemon {
public:
    explicit PywGDaemon() = default;
    ~PywGDaemon() override = default;

protected:
    CVoid daemonTask(DaoGraph::GDaemonParamPtr param) override {
        PYBIND11_OVERLOAD_PURE(CVoid, GDaemon, daemonTask, param);
    }

    CMSec modify(DaoGraph::GDaemonParamPtr param) override {
        PYBIND11_OVERLOAD(CMSec, GDaemon, modify, param);
    }
};

#endif //DAOGRAPH_PYWGDAEMON_H
