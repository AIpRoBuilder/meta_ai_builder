/***************************

@File: GScheduleDefine.h
@Time: 2023/3/25 19:20
@Desc: 
***************************/

#ifndef DAOGRAPH_GSCHEDULEDEFINE_H
#define DAOGRAPH_GSCHEDULEDEFINE_H

#include "../../GraphDefine.h"

DAOGRAPH_NAMESPACE_BEGIN
DAOGRAPH_INTERNAL_NAMESPACE_BEGIN

enum class GScheduleType {
    UNIQUE = 0,        // 独占式，一个pipeline 独占一个 thread pool，默认的类型
    SHARED = 1,        // 共享式，n个pipeline 共享m个 thread pool
};

DAOGRAPH_INTERNAL_NAMESPACE_END
DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GSCHEDULEDEFINE_H
