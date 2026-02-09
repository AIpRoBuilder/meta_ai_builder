/***************************

@File: PywGCluster.h
@Time: 2025/4/6 19:00
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGCLUSTER_H
#define DAOGRAPH_PYWGCLUSTER_H

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGCluster : public DaoGraph::GCluster {
public:
    explicit PywGCluster(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) {
        __addGElements_4py(elements);
    }

    ~PywGCluster() override = default;

    DaoGraph::GElement* clone() const override {
        PYBIND11_OVERLOAD(DaoGraph::GElement*, DaoGraph::GCluster, clone);
    }

protected:
    std::string getClassName() const override {
        return this->getName();
    }
};

PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGCluster, ;);

#endif //DAOGRAPH_PYWGCLUSTER_H
