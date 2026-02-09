/***************************

@File: GMessageObject.h
@Time: 2022/10/30 20:37
@Desc: 
***************************/

#ifndef DAOGRAPH_GMESSAGEOBJECT_H
#define DAOGRAPH_GMESSAGEOBJECT_H

#include "../GraphObject.h"
#include "../GraphParam/GParamInclude.h"

DAOGRAPH_NAMESPACE_BEGIN

class GMessageObject : public GraphObject {
public:
    CStatus run() final {
        DAOGRAPH_NO_SUPPORT
    }
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GMESSAGEOBJECT_H
