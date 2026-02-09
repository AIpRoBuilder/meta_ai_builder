/***************************

@File: URandomDefine.h
@Time: 2022/5/14 11:46
@Desc: 
***************************/

#ifndef DAOGRAPH_URANDOMDEFINE_H
#define DAOGRAPH_URANDOMDEFINE_H

#include <random>

#include "../UtilsObject.h"

DAOGRAPH_NAMESPACE_BEGIN

using DAOGRAPH_RANDOM_MT19937 = std::mt19937;                // 梅森旋转法
using DAOGRAPH_RANDOM_MINSTD_RAND = std::minstd_rand;        // 线性同余法
using DAOGRAPH_RANDOM_RANLUX24_BASE = std::ranlux24_base;    // 滞后Fibonacci

const static CInt DAOGRAPH_REAL_RANDOM = 0;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_URANDOMDEFINE_H
