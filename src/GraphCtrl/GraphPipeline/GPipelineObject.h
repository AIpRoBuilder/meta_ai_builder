/***************************

@File: GPipelineObject.h
@Time: 2023/1/12 23:12
@Desc: 
***************************/

#ifndef DAOGRAPH_GPIPELINEOBJECT_H
#define DAOGRAPH_GPIPELINEOBJECT_H

#include "../GraphObject.h"

DAOGRAPH_NAMESPACE_BEGIN

class GPipelineObject : public GraphObject {
protected:
    CBool is_init_ = false;                          // 判断是否init
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GPIPELINEOBJECT_H
