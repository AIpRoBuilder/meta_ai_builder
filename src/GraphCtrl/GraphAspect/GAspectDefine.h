/***************************

@File: GAspectDefine.h
@Time: 2021/10/3 9:44 上午
@Desc: 
***************************/

#ifndef DAOGRAPH_GASPECTDEFINE_H
#define DAOGRAPH_GASPECTDEFINE_H

#include "../GraphObject.h"

DAOGRAPH_NAMESPACE_BEGIN
DAOGRAPH_INTERNAL_NAMESPACE_BEGIN

enum class GAspectType {
    BEGIN_INIT = 0,
    FINISH_INIT = 1,
    BEGIN_RUN = 2,
    FINISH_RUN = 3,
    BEGIN_DESTROY = 4,
    FINISH_DESTROY = 5,
    ENTER_TIMEOUT = 98,
    ENTER_CRASHED = 99,
};

DAOGRAPH_INTERNAL_NAMESPACE_END
DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GASPECTDEFINE_H
