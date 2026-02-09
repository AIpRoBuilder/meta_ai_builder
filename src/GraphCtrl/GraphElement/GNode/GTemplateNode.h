/***************************

@File: GTemplateNode.h
@Time: 2022/12/24 12:21
@Desc: 
***************************/

#ifndef DAOGRAPH_GTEMPLATENODE_H
#define DAOGRAPH_GTEMPLATENODE_H

#include "GNode.h"

DAOGRAPH_NAMESPACE_BEGIN

template<typename ...Args>
class GTemplateNode : public GNode {
};

template<typename ...Args>
using GTemplateNodePtr = GTemplateNode<Args ...> *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GTEMPLATENODE_H
