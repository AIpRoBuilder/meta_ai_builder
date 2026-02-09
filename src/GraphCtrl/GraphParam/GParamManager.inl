/***************************

@File: GParamManager.inl
@Time: 2021/6/11 6:59 下午
@Desc:
***************************/

#ifndef DAOGRAPH_GPARAMMANAGER_INL
#define DAOGRAPH_GPARAMMANAGER_INL

#include <typeinfo>

#include "GParamManager.h"

DAOGRAPH_NAMESPACE_BEGIN

template<typename T,
        c_enable_if_t<std::is_base_of<GParam, T>::value, int>>
CStatus GParamManager::create(const std::string& key, CBool backtrace) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);
    auto iter = params_map_.find(key);
    if (iter != params_map_.end()) {
        /* 如果是重复创建，则返回ok；非重复创建（类型不同）则返回err */
        auto param = iter->second;
        return (typeid(*param).name() == typeid(T).name()) ?
               CStatus() : CStatus("create [" + key + "] param duplicate");
    }

    T* ptr = DAOGRAPH_SAFE_MALLOC_COBJECT(T)
    ((GParamPtr)ptr)->key_ = key;
    ((GParamPtr)ptr)->backtrace_enable_ = backtrace;
    params_map_.insert(std::pair<std::string, T*>(key, ptr));
    DAOGRAPH_FUNCTION_END
}


template<typename T,
        c_enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GParamManager::get(const std::string& key) {
    const auto& iter = params_map_.find(key);
    if (iter == params_map_.end()) {
        return nullptr;
    }

    /**
     * 实测比 return dynamic_cast<T *>(iter->second); 快很多
     * dynamic_cast<T *> : 当前方案，耗时比约为 10:3
     */
    auto param = iter->second;
    return likely(typeid(T) == typeid(*param)) ? static_cast<T *>(param) : nullptr;
}

DAOGRAPH_NAMESPACE_END

#endif // DAOGRAPH_GPARAMMANAGER_INL