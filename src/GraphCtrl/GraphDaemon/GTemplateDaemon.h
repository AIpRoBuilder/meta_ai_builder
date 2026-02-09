/***************************

@File: GTemplateDaemon.h
@Time: 2022/12/23 23:39
@Desc: 
***************************/

#ifndef DAOGRAPH_GTEMPLATEDAEMON_H
#define DAOGRAPH_GTEMPLATEDAEMON_H

#include "GDaemon.h"

DAOGRAPH_NAMESPACE_BEGIN

template<class ...Args>
class GTemplateDaemon : public GDaemon {
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GTEMPLATEDAEMON_H
