/***************************

@File: PywGParallelMultiCondition.h
@Time: 2025/4/8 00:09
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGPARALLELMULTICONDITION_H
#define DAOGRAPH_PYWGPARALLELMULTICONDITION_H

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGParallelMultiCondition : public DaoGraph::GMultiCondition<DaoGraph::GMultiConditionType::PARALLEL> {
public:
    explicit PywGParallelMultiCondition(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) {
        DaoGraph::GMultiCondition<DaoGraph::GMultiConditionType::PARALLEL>::__addGElements_4py(elements);
    };

    ~PywGParallelMultiCondition() override = default;

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GMultiCondition<DaoGraph::GMultiConditionType::PARALLEL>, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGParallelMultiCondition, ;);

#endif //DAOGRAPH_PYWGPARALLELMULTICONDITION_H
