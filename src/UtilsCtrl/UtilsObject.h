/***************************

@File: UtilsObject.h
@Time: 2021/9/19 12:00 上午
@Desc: 
***************************/

#ifndef DAOGRAPH_UTILSOBJECT_H
#define DAOGRAPH_UTILSOBJECT_H

#include "UtilsDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

class UtilsObject : public CObject {
protected:
    CStatus run() override {
        DAOGRAPH_NO_SUPPORT
    }
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UTILSOBJECT_H
