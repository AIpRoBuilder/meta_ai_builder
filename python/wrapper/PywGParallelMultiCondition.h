/***************************

@File: PywGParallelMultiCondition.h
@Time: 2025/4/8 00:09
@Desc: 
***************************/

#ifndef CGRAPH_PYWGPARALLELMULTICONDITION_H
#define CGRAPH_PYWGPARALLELMULTICONDITION_H

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGParallelMultiCondition : public CGraph::GMultiCondition<CGraph::GMultiConditionType::PARALLEL> {
public:
    explicit PywGParallelMultiCondition(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        CGraph::GMultiCondition<CGraph::GMultiConditionType::PARALLEL>::__addGElements_4py(elements);
    };

    ~PywGParallelMultiCondition() override = default;

    CGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(CGraph::GElement*, CGraph::GMultiCondition<CGraph::GMultiConditionType::PARALLEL>, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGParallelMultiCondition, ;);

#endif //CGRAPH_PYWGPARALLELMULTICONDITION_H
