/***************************

@File: PywGElement.h
@Time: 2025/1/30 21:39
@Desc: 
***************************/

#ifndef CGRAPH_PYWGELEMENT_H
#define CGRAPH_PYWGELEMENT_H

#include <pybind11/pybind11.h>

#include "CGraph.h"

class PywGElement : public CGraph::GElement {
protected:
    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GElement, run);
    }

    std::string getClassName() const override {
        return this->getName();
    }

public:
    GElement* clone() const override {
        PYBIND11_OVERLOAD(GElement*, GElement, clone);
    }
};

#endif //CGRAPH_PYWGELEMENT_H
