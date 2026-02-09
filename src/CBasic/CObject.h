/***************************

@File: CObject.h
@Time: 2021/4/26 8:12 下午
@Desc: 所有类型的父节点，其中run()方法必须实现
***************************/

#ifndef DAOGRAPH_COBJECT_H
#define DAOGRAPH_COBJECT_H

#include "CBasicDefine.h"
#include "CValType.h"
#include "CFuncType.h"

DAOGRAPH_NAMESPACE_BEGIN

class CObject {
public:
    /**
     * 默认构造函数
     */
    explicit CObject() = default;

    /**
     * 初始化函数
     */
    virtual CStatus init() {
        DAOGRAPH_EMPTY_FUNCTION
    }

    /**
     * 流程处理函数
     */
    virtual CStatus run() = 0;

    /**
     * 释放函数
     */
    virtual CStatus destroy() {
        DAOGRAPH_EMPTY_FUNCTION
    }

    /**
     * 默认析构函数
     */
    virtual ~CObject() = default;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_COBJECT_H
