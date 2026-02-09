/***************************

@File: PywGFence.h
@Time: 2025/4/11 00:05
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGFENCE_H
#define DAOGRAPH_PYWGFENCE_H


#include "DaoGraph.h"

class PywGFence : public DaoGraph::GFence {
public:
    explicit PywGFence() = default;

    ~PywGFence() override = default;

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GFence, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

#endif //DAOGRAPH_PYWGFENCE_H
