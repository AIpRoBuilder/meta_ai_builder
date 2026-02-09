/***************************

@File: GAspectManager.h
@Time: 2021/10/2 10:17 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GASPECTMANAGER_H
#define DAOGRAPH_GASPECTMANAGER_H

#include "../GraphManager.h"
#include "GAspectObject.h"
#include "GAspect.h"

DAOGRAPH_NAMESPACE_BEGIN

class GAspectManager : public GAspectObject,
                       public GraphManager<GAspect> {
protected:
    explicit GAspectManager();

    ~GAspectManager() override ;

    /**
     * 执行切面逻辑
     * @param type
     * @param curStatus
     * @return
     */
    CStatus reflect(const internal::GAspectType& type,
                    const CStatus& curStatus = CStatus());

    CStatus add(GAspectPtr aspect) final;

    CSize getSize() const final;

    CStatus clear() final;

    /**
     * 弹出去最后一个
     * @return
     */
    CStatus popLast();

    CVoidPtr setGParamManager(GParamManagerPtr pm) final;

    CVoidPtr setGEventManager(GEventManagerPtr em) final;

    CVoidPtr setBelong(GElement* belong);

    DAOGRAPH_NO_ALLOWED_COPY(GAspectManager)

private:
    GAspectPtrArr aspect_arr_ {};     // 存储aspect的容器

    friend class GElement;
    friend class CAllocator;
    friend class GElementManager;
    friend class GStorage;
    friend struct _GElementStorage;
};

using GAspectManagerPtr = GAspectManager *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GASPECTMANAGER_H
