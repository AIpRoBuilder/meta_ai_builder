/***************************

@File: GRegion.h
@Time: 2021/6/1 10:14 下午
@Desc: 实现多个element，根据依赖关系执行的功能
***************************/


#ifndef DAOGRAPH_GREGION_H
#define DAOGRAPH_GREGION_H

#include "../GGroup.h"
#include "../../GElementManager.h"

DAOGRAPH_NAMESPACE_BEGIN

class GRegion : public GGroup {
public:
    /**
     * 设置EngineType信息
     * @param type
     * @return
     */
    GRegion* setGEngineType(GEngineType type);

    /**
     * 修剪冗余的连边信息
     * @return
     */
    CSize trim();

protected:
    explicit GRegion();
    ~GRegion() override;

    CStatus init() final;
    CStatus run() final;
    CStatus destroy() final;

private:
    CVoid dump(std::ostream& oss) final;

    CBool isSerializable() const final;

    CStatus addElementEx(GElementPtr element) final;

    GElementPtr setThreadPoolEx(UThreadPoolPtr ptr) final;

    CBool isSeparate(GElementCPtr a, GElementCPtr b) const final;

private:
    GElementManagerPtr manager_ = nullptr;    // region 内部通过 manager来管理其中的 element 信息

    DAOGRAPH_NO_ALLOWED_COPY(GRegion)

    friend class GPipeline;
    friend class CAllocator;
    friend class GStorageFactory;
    friend class GTrimOptimizer;
};

using GRegionPtr = GRegion *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GREGION_H
