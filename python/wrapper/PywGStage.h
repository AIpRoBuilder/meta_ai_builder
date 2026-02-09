/***************************

@File: PywGStage.h
@Time: 2025/4/4 00:47
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGSTAGE_H
#define DAOGRAPH_PYWGSTAGE_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"

class PywGStage : public DaoGraph::GStage {
public:
    explicit PywGStage() = default;
    ~PywGStage() override = default;

protected:
    CVoid launch(DaoGraph::GStageParamPtr param) override {
        PYBIND11_OVERLOAD(CVoid, GStage, launch, param);
    }
};

#endif //DAOGRAPH_PYWGSTAGE_H
