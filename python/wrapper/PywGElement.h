/***************************

@File: PywGElement.h
@Time: 2025/1/30 21:39
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGELEMENT_H
#define DAOGRAPH_PYWGELEMENT_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"

class PywGElement : public DaoGraph::GElement {
protected:
    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GElement, run);
    }

    CStatus onInput(const std::string& input) override {
        PYBIND11_OVERLOAD(CStatus, GElement, onInput, input);
    }

    CStatus onInputs(const std::vector<std::string>& inputs) override {
        PYBIND11_OVERLOAD(CStatus, GElement, onInputs, inputs);
    }

    std::string getClassName() const override {
        return this->getName();
    }

public:
    GElement* clone() const override {
        PYBIND11_OVERLOAD(GElement*, GElement, clone);
    }
};

#endif //DAOGRAPH_PYWGELEMENT_H
