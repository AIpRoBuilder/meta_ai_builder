/***************************

@File: PywGFunction.h
@Time: 2025/4/13 18:38
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGFUNCTION_H
#define DAOGRAPH_PYWGFUNCTION_H


#include "DaoGraph.h"

class PywGFunction : public DaoGraph::GFunction {
public:
    explicit PywGFunction() = default;

    ~PywGFunction() override = default;

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GFunction, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

#endif //DAOGRAPH_PYWGFUNCTION_H
