/***************************

@File: GAspectObject.inl
@Time: 2021/10/2 9:46 下午
@Desc:
***************************/

#ifndef DAOGRAPH_GASPECTOBJECT_INL
#define DAOGRAPH_GASPECTOBJECT_INL

#include "GAspectObject.h"

DAOGRAPH_NAMESPACE_BEGIN

template <typename T,
          c_enable_if_t<std::is_base_of<GAspectParam, T>::value, int>>
GAspectObjectPtr GAspectObject::setAParam(T* param) {
    /** 传入的param可以为空 */
    if (param) {
        DAOGRAPH_DELETE_PTR(param_)
        param_ = DAOGRAPH_SAFE_MALLOC_COBJECT(T)    // 确保param是最新的
        param_->clone(static_cast<T *>(param));
    }

    return this;
}


template <typename T,
          c_enable_if_t<std::is_base_of<GAspectParam, T>::value, int>>
T* GAspectObject::getAParam() {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_)

    T* param = nullptr;
    if ((typeid(*param_).name() == typeid(T).name())) {
        // 如果类型相同才可以获取成功，否则直接返回nullptr
        param = dynamic_cast<T *>(this->param_);
    }
    return param;
}

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GASPECTOBJECT_INL
