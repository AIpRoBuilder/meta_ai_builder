/***************************

@File: UCvMutex.h
@Time: 2024/12/13 20:27
@Desc: 
***************************/

#ifndef DAOGRAPH_UCVMUTEX_H
#define DAOGRAPH_UCVMUTEX_H

#include <mutex>
#include <condition_variable>

#include "../UThreadObject.h"

DAOGRAPH_NAMESPACE_BEGIN

struct UCvMutex : public UThreadObject {
    std::mutex mtx_ {};
    std::condition_variable cv_ {};
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UCVMUTEX_H
