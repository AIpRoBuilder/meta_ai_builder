/***************************

@File: CThreadObject.h
@Time: 2021/7/2 10:39 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_UTHREADOBJECT_H
#define DAOGRAPH_UTHREADOBJECT_H

#include "../UtilsObject.h"
#include "UThreadPoolDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

class UThreadObject : public UtilsObject {
protected:
    /**
     * 部分thread中的算子，可以不实现run方法
     * @return
     */
    CStatus run() override {
        DAOGRAPH_NO_SUPPORT
    }
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UTHREADOBJECT_H
