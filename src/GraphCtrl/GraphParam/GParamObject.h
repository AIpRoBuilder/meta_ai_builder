/***************************

@File: GParamObject.h
@Time: 2021/9/29 9:23 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GPARAMOBJECT_H
#define DAOGRAPH_GPARAMOBJECT_H

#include "../GraphObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

DAOGRAPH_NAMESPACE_BEGIN

class GParamObject : public GraphObject {
protected:
    /**
     * GParam相关内容，无法通过run()方法执行
     * @return
     */
    CStatus run() final {
        DAOGRAPH_NO_SUPPORT
    }
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GPARAMOBJECT_H
