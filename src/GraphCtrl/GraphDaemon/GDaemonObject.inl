/***************************

@File: GDaemonObject.inl
@Time: 2022/12/18 9:36 下午
@Desc:
***************************/

#ifndef DAOGRAPH_GDAEMONOBJECT_INL
#define DAOGRAPH_GDAEMONOBJECT_INL

#include "GDaemonObject.h"

DAOGRAPH_NAMESPACE_BEGIN

template <typename DParam,
        c_enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int>>
GDaemonObjectPtr GDaemonObject::setDParam(DParam* param) {
    if (param) {
        DAOGRAPH_DELETE_PTR(param_)
        param_ = DAOGRAPH_SAFE_MALLOC_COBJECT(DParam)    // 确保param是最新的
        param_->clone(static_cast<DParam *>(param));
    }

    return this;
}

DAOGRAPH_NAMESPACE_END

#endif // DAOGRAPH_GDAEMONOBJECT_INL