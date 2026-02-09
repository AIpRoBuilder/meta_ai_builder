/***************************

@File: GCluster.h
@Time: 2021/6/1 10:14 下午
@Desc: 实现多个element，线性执行的功能
***************************/

#ifndef DAOGRAPH_GCLUSTER_H
#define DAOGRAPH_GCLUSTER_H

#include <vector>

#include "../GGroup.h"

DAOGRAPH_NAMESPACE_BEGIN

class GCluster : public GGroup {
protected:
    explicit GCluster();

private:
    CStatus run() final;

    CVoid dump(std::ostream& oss) final;

    CBool isSeparate(GElementCPtr a, GElementCPtr b) const final;

    friend class GPipeline;
    friend class GDynamicEngine;
    friend class CAllocator;
    friend class GStorageFactory;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GCLUSTER_H
