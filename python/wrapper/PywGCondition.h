/***************************

@File: PywGConditiion.h
@Time: 2025/1/30 21:38
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGCONDITION_H
#define DAOGRAPH_PYWGCONDITION_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGCondition : public DaoGraph::GCondition {
public:
    explicit PywGCondition(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    }
    ~PywGCondition() override = default;

    CIndex choose() override {
        PYBIND11_OVERLOAD_PURE(CIndex, PywGCondition, choose);
    }

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GCondition, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGCondition,
    CIndex choose() override {
        PYBIND11_OVERLOAD_PURE(CIndex, PywGConditionInterface, choose);
    });

#endif //DAOGRAPH_PYWGCONDITION_H
