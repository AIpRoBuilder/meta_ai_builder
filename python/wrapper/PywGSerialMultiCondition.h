/***************************

@File: PywGSerialMultiCondition.h
@Time: 2025/4/8 00:05
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGSERIALMULTICONDITION_H
#define DAOGRAPH_PYWGSERIALMULTICONDITION_H

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGSerialMultiCondition : public DaoGraph::GMultiCondition<DaoGraph::GMultiConditionType::SERIAL> {
public:
    explicit PywGSerialMultiCondition(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) {
        DaoGraph::GMultiCondition<DaoGraph::GMultiConditionType::SERIAL>::__addGElements_4py(elements);
    };

    ~PywGSerialMultiCondition() override = default;

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GMultiCondition<DaoGraph::GMultiConditionType::SERIAL>, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGSerialMultiCondition, ;);

#endif //DAOGRAPH_PYWGSERIALMULTICONDITION_H
