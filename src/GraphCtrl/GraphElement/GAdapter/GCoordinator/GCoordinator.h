/***************************

@File: GCoordinator.h
@Time: 2023/9/8 21:12
@Desc: 
***************************/

#ifndef DAOGRAPH_GCOORDINATOR_H
#define DAOGRAPH_GCOORDINATOR_H

#include "../GAdapter.h"

DAOGRAPH_NAMESPACE_BEGIN

template<CInt SIZE>
class GCoordinator : public GAdapter {
protected:
    explicit GCoordinator();

    CStatus run() final;

    CVoid dumpElement(std::ostream& oss) final;

    CStatus checkSuitable() final;

    friend class GPipeline;
};

template<CInt SIZE>
using GCoordinatorPtr = GCoordinator<SIZE> *;

template<CInt SIZE>
using GCoordinatorPPtr = GCoordinatorPtr<SIZE> *;

DAOGRAPH_NAMESPACE_END

#include "GCoordinator.inl"

#endif //DAOGRAPH_GCOORDINATOR_H
