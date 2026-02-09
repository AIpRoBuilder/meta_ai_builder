/***************************

@File: GParamUtils.h
@Time: 2022/11/1 23:37
@Desc:
***************************/

#ifndef DAOGRAPH_GPARAMUTILS_H
#define DAOGRAPH_GPARAMUTILS_H

#include "../GraphObject.h"

DAOGRAPH_NAMESPACE_BEGIN

/** 创建参数信息 */
#define DAOGRAPH_CREATE_GPARAM(Type, key)                                       \
    this->template createGParam<Type>(key);                                   \

#define DAOGRAPH_CREATE_GPARAM_WITH_BACKTRACE(Type, key)                        \
    this->template createGParam<Type>(key, true);                             \

/** 获取参数信息 */
#define DAOGRAPH_GET_GPARAM(Type, key)                                          \
    this->template getGParam<Type>(key);                                      \

/** 获取参数信息，为空则抛出异常 */
#define DAOGRAPH_GET_GPARAM_WITH_NO_EMPTY(Type, key)                            \
    this->template getGParamWithNoEmpty<Type>(key);                           \

/** 删除一个参数 */
#define DAOGRAPH_DELETE_GPARAM(key)                                             \
    this->removeGParam(key);                                                  \

/** 获取element内部参数信息 */
#define DAOGRAPH_GET_EPARAM(Type, key)                                          \
    this->template getEParam<Type>(key);                                      \

/** 上参数写锁 */
#define DAOGRAPH_PARAM_WRITE_CODE_BLOCK(param)                                  \
    DaoGraph::DAOGRAPH_WRITE_LOCK __paramWLock__((param)->_param_shared_lock_);   \

/** 上参数读锁 */
#define DAOGRAPH_PARAM_READ_CODE_BLOCK(param)                                   \
    DaoGraph::DAOGRAPH_READ_LOCK __paramRLock__((param)->_param_shared_lock_);    \

DAOGRAPH_NAMESPACE_END


#endif //DAOGRAPH_GPARAMUTILS_H
