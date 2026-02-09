/***************************

@File: GConditionDefine.h
@Time: 2023/6/17 19:24
@Desc: 
***************************/

#ifndef DAOGRAPH_GCONDITIONDEFINE_H
#define DAOGRAPH_GCONDITIONDEFINE_H

#include "../GGroupDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

enum class GMultiConditionType {
    SERIAL = 0,        // 串行执行
    PARALLEL = 1,      // 并行执行
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GCONDITIONDEFINE_H
