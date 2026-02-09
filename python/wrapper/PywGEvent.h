/***************************

@File: PywGEvent.h
@Time: 2025/3/19 23:35
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGEVENT_H
#define DAOGRAPH_PYWGEVENT_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"

class PywGEvent : public DaoGraph::GEvent {
public:
    explicit PywGEvent() = default;

protected:
    CVoid trigger(DaoGraph::GEventParamPtr param) override {
        PYBIND11_OVERLOAD_PURE(CVoid, GEvent, trigger, param);
    }

    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, GEvent, init);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, GEvent, destroy);
    }
};

#endif //DAOGRAPH_PYWGEVENT_H
