/***************************

@File: DAnnObject.h
@Time: 2022/4/4 21:17
@Desc: 所有的Ann抽象接口类
***************************/

#ifndef DAOGRAPH_DANNOBJECT_H
#define DAOGRAPH_DANNOBJECT_H

#include "../DomainObject.h"

DAOGRAPH_NAMESPACE_BEGIN

class DAnnObject : public DomainObject {
protected:
    CStatus run() override {
        DAOGRAPH_NO_SUPPORT
    }
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_DANNOBJECT_H
