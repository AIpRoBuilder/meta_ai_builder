/***************************

@File: PywGMutable.h
@Time: 2025/4/8 23:19
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGMUTABLE_H
#define DAOGRAPH_PYWGMUTABLE_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGMutable : public DaoGraph::GMutable {
public:
    explicit PywGMutable(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    }
    ~PywGMutable() override = default;

    CStatus reshape(DaoGraph::GElementPtrArr& elements) override {
        PYBIND11_OVERLOAD_PURE(CStatus, PywGMutable, reshape, elements);
    }

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GMutable, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGMutable,
    CStatus reshape(DaoGraph::GElementPtrArr& elements) override {
        PYBIND11_OVERLOAD_PURE(CStatus, PywGMutableInterface, reshape, elements);
    });

#endif //DAOGRAPH_PYWGMUTABLE_H
