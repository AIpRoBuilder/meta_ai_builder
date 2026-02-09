/***************************

@File: GEventManager.cpp
@Time: 2023/1/20 23:08
@Desc: 
***************************/

#include "GEventManager.h"

DAOGRAPH_NAMESPACE_BEGIN

CStatus GEventManager::init() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        // fatInit 中包含了init()，和部分其他逻辑
        auto event = iter.second;
        event->setGParamManager(param_manager_);
        status += event->fatInit();
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GEventManager::destroy() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        status += (iter.second)->fatDestroy();
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GEventManager::clear() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        DAOGRAPH_DELETE_PTR(iter.second)
    }

    events_map_.clear();
    DAOGRAPH_FUNCTION_END
}


CStatus GEventManager::trigger(const std::string &key, GEventType type, GEventAsyncStrategy strategy) {
    DAOGRAPH_FUNCTION_BEGIN
    auto result = events_map_.find(key);
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(events_map_.end() == result,
                                            "event key [" + key + "] no find")

    auto event = result->second;
    DAOGRAPH_ASSERT_NOT_NULL(event)
    status = event->process(type, strategy);
    DAOGRAPH_FUNCTION_END
}


std::shared_future<CVoid> GEventManager::asyncTrigger(const std::string &key, GEventAsyncStrategy strategy) {
    auto result = events_map_.find(key);
    DAOGRAPH_THROW_EXCEPTION_BY_CONDITION(events_map_.end() == result || !result->second,
                                        "event key [" + key + "] no find");

    auto event = result->second;
    return event->asyncProcess(strategy);
}


GEventObjectPtr GEventManager::setThreadPool(UThreadPoolPtr ptr) {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr)
    for (auto& iter : events_map_) {
        DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(iter.second)
        (iter.second)->setThreadPool(ptr);
    }

    return this;
}


GEventManager::~GEventManager() {
    clear();
}


CStatus GEventManager::reset() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& iter : events_map_) {
        iter.second->asyncWait(GEventAsyncStrategy::PIPELINE_RUN_FINISH);
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GEventManager::__create_4py(GEventPtr event, const std::string& key) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(event)
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(events_map_.find(key) != events_map_.end(),
                                            "event key [" + key + "] duplicate.");

    events_map_[key] = event;
    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END