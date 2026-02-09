/***************************

@File: GStageObject.h
@Time: 2024/12/14 11:26
@Desc: 
***************************/

#ifndef DAOGRAPH_GSTAGEOBJECT_H
#define DAOGRAPH_GSTAGEOBJECT_H

#include <atomic>

#include "../GraphObject.h"
#include "../GraphParam/GParamInclude.h"

DAOGRAPH_NAMESPACE_BEGIN

class GStageObject : public GraphObject,
                     public CDescInfo {
private:
    CStatus run() final {
        DAOGRAPH_NO_SUPPORT
    }
};

DAOGRAPH_NAMESPACE_END


#endif //DAOGRAPH_GSTAGEOBJECT_H
