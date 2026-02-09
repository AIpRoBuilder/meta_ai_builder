/***************************

@File: PywGParam.h
@Time: 2025/2/1 14:04
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGPARAM_H
#define DAOGRAPH_PYWGPARAM_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"

class PywGParam : public DaoGraph::GParam {
public:
    ~PywGParam() override = default;

protected:
    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, DaoGraph::GParam, init);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, DaoGraph::GParam, destroy);
    }

    CStatus setup() override {
        PYBIND11_OVERLOAD(CStatus, DaoGraph::GParam, setup);
    }

    CVoid reset(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, DaoGraph::GParam, reset, curStatus);
    }
};

#endif //DAOGRAPH_PYWGPARAM_H
