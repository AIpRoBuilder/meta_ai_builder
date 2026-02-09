/***************************

@File: PywGAspect.h
@Time: 2025/3/5 23:06
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGASPECT_H
#define DAOGRAPH_PYWGASPECT_H

#include <pybind11/pybind11.h>

#include "DaoGraph.h"

class PywGAspect : public DaoGraph::GAspect {
public:
    explicit PywGAspect() = default;
    ~PywGAspect() override = default;

protected:
    CStatus beginInit() override {
        PYBIND11_OVERLOAD(CStatus, GAspect, beginInit);
    }

    CVoid finishInit(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishInit, curStatus);
    }

    CStatus beginRun() override {
        PYBIND11_OVERLOAD(CStatus, GAspect, beginRun);
    }

    CVoid finishRun(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishRun, curStatus);
    }

    CStatus beginDestroy() override {
        PYBIND11_OVERLOAD(CStatus, GAspect, beginDestroy);
    }

    CVoid finishDestroy(const CStatus& curStatus) override {
        PYBIND11_OVERLOAD(CVoid, GAspect, finishDestroy, curStatus);
    }

    CVoid enterCrashed() override {
        PYBIND11_OVERLOAD(CVoid, GAspect, enterCrashed);
    }

    CVoid enterTimeout() override {
        PYBIND11_OVERLOAD(CVoid, GAspect, enterTimeout);
    }
};

#endif //DAOGRAPH_PYWGASPECT_H
