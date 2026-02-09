/***************************

@File: GEventManager.inl
@Time: 2023/1/17 22:56
@Desc:
***************************/

#ifndef DAOGRAPH_GEVENTMANAGER_INL
#define DAOGRAPH_GEVENTMANAGER_INL

#include "GEventManager.h"

DAOGRAPH_NAMESPACE_BEGIN

template<typename TEvent, typename TEParam,
        c_enable_if_t<std::is_base_of<GEvent, TEvent>::value, int>,
        c_enable_if_t<std::is_base_of<GEventParam, TEParam>::value, int>>
CStatus GEventManager::createWithParam(const std::string& key, TEParam* param) {
    DAOGRAPH_FUNCTION_BEGIN
    auto result = events_map_.find(key);
    if (events_map_.end() != result) {
        // 如果注册过了，则直接返回注册失败
        DAOGRAPH_RETURN_ERROR_STATUS("event [" + key + "] has been register")
    }

    GEventPtr event = DAOGRAPH_SAFE_MALLOC_COBJECT(TEvent)

    // 每次创建，都是重新生成参数的一个过程
    DAOGRAPH_DELETE_PTR(event->param_)
    if (nullptr != param) {
        event->param_ = DAOGRAPH_SAFE_MALLOC_COBJECT(TEParam);
        event->param_->clone(param);
    }
    events_map_[key] = event;
    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END

#endif // DAOGRAPH_GEVENTMANAGER_INL
