/***************************

@File: GTopoEngine.h
@Time: 2024/1/20 21:56
@Desc: 
***************************/

#ifndef DAOGRAPH_GTOPOENGINE_H
#define DAOGRAPH_GTOPOENGINE_H

#include <vector>
#include <set>

#include "../GEngine.h"

DAOGRAPH_NAMESPACE_BEGIN

class GTopoEngine : public GEngine {
protected:
    explicit GTopoEngine() = default;

    CStatus setup(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

private:
    GElementPtrArr topo_elements_ {};       // 拓扑排序后的

    friend class CAllocator;
    friend class GElementManager;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GTOPOENGINE_H
