/***************************

@File: PywGPassedParam.h
@Time: 2025/3/22 13:22
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGPASSEDPARAM_H
#define DAOGRAPH_PYWGPASSEDPARAM_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"

class PywGPassedParam : public DaoGraph::GPassedParam {
public:
    explicit PywGPassedParam() = default;
    ~PywGPassedParam() override = default;

protected:
    CVoid clone(GPassedParam* param) override {
        PYBIND11_OVERLOAD_PURE(CVoid, GPassedParam, clone, param);
    }
};

#endif //DAOGRAPH_PYWGPASSEDPARAM_H
