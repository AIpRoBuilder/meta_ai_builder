/***************************

@File: PywGSerialMultiCondition.h
@Time: 2025/4/8 00:05
@Desc: 
***************************/

#ifndef CGRAPH_PYWGSERIALMULTICONDITION_H
#define CGRAPH_PYWGSERIALMULTICONDITION_H

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGSerialMultiCondition : public CGraph::GMultiCondition<CGraph::GMultiConditionType::SERIAL> {
public:
    explicit PywGSerialMultiCondition(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        CGraph::GMultiCondition<CGraph::GMultiConditionType::SERIAL>::__addGElements_4py(elements);
    };

    ~PywGSerialMultiCondition() override = default;

    CGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(CGraph::GElement*, CGraph::GMultiCondition<CGraph::GMultiConditionType::SERIAL>, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGSerialMultiCondition, ;);

#endif //CGRAPH_PYWGSERIALMULTICONDITION_H
