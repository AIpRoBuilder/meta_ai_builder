/***************************

@File: GTemplateAspect.h
@Time: 2022/12/23 22:01
@Desc: 用于生成可变构造参数的切面
***************************/

#ifndef DAOGRAPH_GTEMPLATEASPECT_H
#define DAOGRAPH_GTEMPLATEASPECT_H

#include "GAspect.h"

DAOGRAPH_NAMESPACE_BEGIN

template<class ...Args>
class GTemplateAspect : public GAspect {
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GTEMPLATEASPECT_H
