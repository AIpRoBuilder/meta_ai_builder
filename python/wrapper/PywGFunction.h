/***************************

@File: PywGFunction.h
@Time: 2025/4/13 18:38
@Desc: 
***************************/

#ifndef CGRAPH_PYWGFUNCTION_H
#define CGRAPH_PYWGFUNCTION_H


#include "CGraph.h"

class PywGFunction : public CGraph::GFunction {
public:
    explicit PywGFunction() = default;

    ~PywGFunction() override = default;

    CGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(CGraph::GElement*, CGraph::GFunction, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

#endif //CGRAPH_PYWGFUNCTION_H
