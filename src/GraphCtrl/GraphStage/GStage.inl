/***************************

@File: GStage.inl
@Time: 2024/12/30 9:46 下午
@Desc:
***************************/

#ifndef DAOGRAPH_GSTAGE_INL
#define DAOGRAPH_GSTAGE_INL

#include "GStage.h"

DAOGRAPH_NAMESPACE_BEGIN

template <typename T,
        c_enable_if_t<std::is_base_of<GStageParam, T>::value, int>>
GStagePtr GStage::setSParam(T* param) {
    if (param) {
        DAOGRAPH_DELETE_PTR(param_);
        param_ = DAOGRAPH_SAFE_MALLOC_COBJECT(T);
        param_->clone(static_cast<T *>(param));
    }

    return this;
}

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GSTAGE_INL