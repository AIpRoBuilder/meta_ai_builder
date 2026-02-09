/***************************

@File: PywGRegion.h
@Time: 2025/4/6 19:03
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGREGION_H
#define DAOGRAPH_PYWGREGION_H

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGRegion : public DaoGraph::GRegion {
public:
    explicit PywGRegion(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    };

    ~PywGRegion() override = default;

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GRegion, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGRegion, ;);

#endif //DAOGRAPH_PYWGREGION_H
