/***************************

@File: UQueueObject.h
@Time: 2022/10/1 20:31
@Desc: 
***************************/

#ifndef DAOGRAPH_UQUEUEOBJECT_H
#define DAOGRAPH_UQUEUEOBJECT_H

#include <mutex>

#include "../UThreadObject.h"
#include "UQueueDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

class UQueueObject : public UThreadObject {
protected:
    std::mutex mutex_;
    std::condition_variable cv_;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UQUEUEOBJECT_H
