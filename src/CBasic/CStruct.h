/***************************

@File: CStruct.h
@Time: 2023/7/16 11:36
@Desc: 
***************************/

#ifndef DAOGRAPH_CSTRUCT_H
#define DAOGRAPH_CSTRUCT_H

#include "CBasicDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

/**
 * 所有框架内部结构体定义的基类
 * 仅针对类似 pod 数据类型的定义
 */
struct CStruct {
    virtual ~CStruct() = default;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_CSTRUCT_H
