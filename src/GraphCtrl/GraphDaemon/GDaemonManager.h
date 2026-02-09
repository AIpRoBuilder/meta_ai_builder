/***************************

@File: GDaemonManager.h
@Time: 2022/2/2 11:17 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GDAEMONMANAGER_H
#define DAOGRAPH_GDAEMONMANAGER_H

#include "GDaemon.h"
#include "../GraphManager.h"

DAOGRAPH_NAMESPACE_BEGIN

class GDaemonManager : public GDaemonObject,
                       public GraphManager<GDaemon> {
protected:
    explicit GDaemonManager() = default;

    ~GDaemonManager() override;

    CStatus init() final;

    CStatus destroy() final;

    CStatus add(GDaemonPtr daemon) final;

    CStatus remove(GDaemonPtr daemon) final;

    CStatus clear() final;

    CSize getSize() const final;

    friend class GPipeline;
    friend class CAllocator;
    friend class GStorage;
    DAOGRAPH_NO_ALLOWED_COPY(GDaemonManager)

private:
    GDaemonSet daemons_ {};                    // daemon信息集合
};

using GDaemonManagerPtr = GDaemonManager *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GDAEMONMANAGER_H
