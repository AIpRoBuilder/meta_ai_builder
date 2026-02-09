/***************************

@File: GStaticEngine.h
@Time: 2024/7/27 10:40
@Desc: 
***************************/

#ifndef DAOGRAPH_GSTATICENGINE_H
#define DAOGRAPH_GSTATICENGINE_H

#include "../GEngine.h"

DAOGRAPH_NAMESPACE_BEGIN

class GStaticEngine : public GEngine {
protected:
    explicit GStaticEngine() = default;

    CStatus setup(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

private:
    GElementPtrMat2D element_mat_ {};        // 解图后的elements信息

    friend class CAllocator;
    friend class GElementManager;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GSTATICENGINE_H
