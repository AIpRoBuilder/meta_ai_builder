/***************************

@File: PywGNode.h
@Time: 2025/1/30 21:41
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGNODE_H
#define DAOGRAPH_PYWGNODE_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"
#include "PyWrapperMacro.h"

class PywGNode : public DaoGraph::GNode {
public:
    explicit PywGNode(const std::string& name,
                      int loop = DaoGraph::DAOGRAPH_DEFAULT_LOOP_TIMES) {
        setName(name);
        setLoop(loop);
    }

    explicit PywGNode(const DaoGraph::GElementPtrSet& depends = DaoGraph::GElementPtrSet{},
                      const std::string& name = DaoGraph::DAOGRAPH_EMPTY,
                      int loop = DaoGraph::DAOGRAPH_DEFAULT_LOOP_TIMES) {
        addDependGElements(depends);
        setLoop(loop);
        setName(name);
    }

    ~PywGNode() override = default;

protected:
    CStatus init() override {
        PYBIND11_OVERLOAD(CStatus, GNode, init);
    }

    CStatus run() override {
        PYBIND11_OVERLOAD_PURE(CStatus, GNode, run);
    }

    CStatus destroy() override {
        PYBIND11_OVERLOAD(CStatus, GNode, destroy);
    }

    PYDAOGRAPH_DECLARE_GELEMENT_COMMON_FUNCTIONS(DaoGraph::GNode)
};

#endif //DAOGRAPH_PYWGNODE_H
