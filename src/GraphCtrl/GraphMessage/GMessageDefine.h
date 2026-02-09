/***************************

@File: GMessageDefine.h
@Time: 2022/12/24 22:16
@Desc: 
***************************/

#ifndef DAOGRAPH_GMESSAGEDEFINE_H
#define DAOGRAPH_GMESSAGEDEFINE_H

#include "GMessageObject.h"

DAOGRAPH_NAMESPACE_BEGIN

using GMessagePushStrategy = URingBufferPushStrategy;

DAOGRAPH_INTERNAL_NAMESPACE_BEGIN
static const char* PUB_SUB_PREFIX = "PS_PRE_";
static const char* SEND_RECV_PREFIX = "SR_PRE_";
DAOGRAPH_INTERNAL_NAMESPACE_END

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GMESSAGEDEFINE_H
