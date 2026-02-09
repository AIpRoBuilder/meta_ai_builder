/***************************

@File: GParamManager.cpp
@Time: 2021/6/11 6:59 下午
@Desc: 
***************************/

#include "GParamManager.h"

DAOGRAPH_NAMESPACE_BEGIN

GParamManager::GParamManager() {
    clear();
}


GParamManager::~GParamManager() {
    clear();
}


CStatus GParamManager::init() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& param : params_map_) {
        /**
         * 一般情况下，是不需要init的。需要注意init 和 setup的执行阶段的信息
         * init() 是在pipeline init的时候被执行的
         * setup() 是在pipeline run开始的时候被执行的
         */
        DAOGRAPH_ASSERT_NOT_NULL(param.second);
        status += param.second->init();
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GParamManager::destroy() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& param : params_map_) {
        DAOGRAPH_ASSERT_NOT_NULL(param.second);
        status += param.second->destroy();
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GParamManager::clear() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);
    for (auto& param : params_map_) {
        DAOGRAPH_DELETE_PTR(param.second)
    }

    params_map_.clear();
    DAOGRAPH_FUNCTION_END;
}


CVoid GParamManager::resetWithStatus(const CStatus& curStatus) {
    for (auto& param : params_map_) {
        param.second->reset(curStatus);
    }
}


CStatus GParamManager::setup() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto& param : params_map_) {
        // 这里不需要判断非空，因为在init的时候，已经判断过了
        status += param.second->setup();
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GParamManager::removeByKey(const std::string& key) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);    // 创建和销毁的时候，加锁
    auto param = params_map_.find(key);
    if (param == params_map_.end()) {
        DAOGRAPH_RETURN_ERROR_STATUS("param [" + key + "] no find")
    }

    DAOGRAPH_DELETE_PTR(param->second)
    params_map_.erase(param);
    DAOGRAPH_FUNCTION_END
}


std::vector<std::string> GParamManager::getKeys() {
    std::vector<std::string> keys;
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);
    keys.reserve(params_map_.size());
    for (const auto& iter : params_map_) {
        keys.emplace_back(iter.first);
    }

    return keys;
}


CStatus GParamManager::__create_4py(GParamPtr param, const std::string& key) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);
    auto iter = params_map_.find(key);
    // python场景中，如果重复添加，仅保留第一次的写入
    if (iter == params_map_.end()) {
        params_map_.insert(std::pair<std::string, GParamPtr>(key, param));
    }

    DAOGRAPH_FUNCTION_END
}


GParamPtr GParamManager::__get_4py(const std::string& key) {
    const auto& iter = params_map_.find(key);
    if (iter == params_map_.end()) {
        return nullptr;
    }

    return iter->second;
}


CStatus GParamManager::__remove_4py(const std::string& key) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);
    auto iter = params_map_.find(key);
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(iter == params_map_.end(),
                                            "param [" + key + "] no find")

    DAOGRAPH_DELETE_PTR(iter->second);
    params_map_.erase(key);

    DAOGRAPH_FUNCTION_END
}


CBool GParamManager::__has_4py(const std::string& key) {
    DAOGRAPH_LOCK_GUARD lock(this->mutex_);
    return params_map_.find(key) != params_map_.end();
}

DAOGRAPH_NAMESPACE_END
