/***************************

@File: GMessageParam.h
@Time: 2022/10/30 23:01
@Desc: 
***************************/

#ifndef DAOGRAPH_GMESSAGEPARAM_H
#define DAOGRAPH_GMESSAGEPARAM_H

#include "GParamObject.h"

DAOGRAPH_NAMESPACE_BEGIN

class GMessageParam : public GParamObject {
    /**
     * 用于消息传递的参数时，建议实现构造拷贝 和 赋值拷贝函数
     * 否则，针对指针类型变量，可能会出现深浅拷贝的问题
     * @param param
     * @return
     */
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GMESSAGEPARAM_H
