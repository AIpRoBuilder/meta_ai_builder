/***************************

@File: GAspectManager.cpp
@Time: 2023/12/29 19:45
@Desc: 
***************************/

#include "GAspectManager.h"

DAOGRAPH_NAMESPACE_BEGIN

GAspectManager::GAspectManager() {
    aspect_arr_ = {};
}


GAspectManager::~GAspectManager() {
    clear();
}


CStatus GAspectManager::reflect(const internal::GAspectType &type,
                                const CStatus &curStatus) {
    DAOGRAPH_FUNCTION_BEGIN

    for (GAspectPtr aspect : aspect_arr_) {
        switch (type) {
            /**
             * 仅针对Begin对应的内容，进行返回值判断
             * run()方法切面更容易被执行，故放在最前方判断
             */
            case internal::GAspectType::BEGIN_RUN:
                status = aspect->beginRun();
                break;
            case internal::GAspectType::FINISH_RUN:
                aspect->finishRun(curStatus);
                break;
            case internal::GAspectType::BEGIN_INIT:
                status = aspect->beginInit();
                break;
            case internal::GAspectType::FINISH_INIT:
                aspect->finishInit(curStatus);
                break;
            case internal::GAspectType::BEGIN_DESTROY:
                status = aspect->beginDestroy();
                break;
            case internal::GAspectType::FINISH_DESTROY:
                aspect->finishDestroy(curStatus);
                break;
            case internal::GAspectType::ENTER_TIMEOUT:
                aspect->enterTimeout();
                break;
            case internal::GAspectType::ENTER_CRASHED:
                aspect->enterCrashed();
                break;
            default:
                status = CStatus("unknown aspect type");
        }

        DAOGRAPH_FUNCTION_CHECK_STATUS
    }

    DAOGRAPH_FUNCTION_END
}


CStatus GAspectManager::add(GAspectPtr aspect) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(aspect)
    aspect_arr_.emplace_back(aspect);
    DAOGRAPH_FUNCTION_END
}


CSize GAspectManager::getSize() const {
    auto size = aspect_arr_.size();
    return size;
}


CStatus GAspectManager::clear() {
    DAOGRAPH_FUNCTION_BEGIN
    for (GAspectPtr aspect: aspect_arr_) {
        DAOGRAPH_DELETE_PTR(aspect)
    }

    aspect_arr_.clear();
    DAOGRAPH_FUNCTION_END
}


CStatus GAspectManager::popLast() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(0 == getSize(), "no aspect to pop")

    auto *last = aspect_arr_.back();
    DAOGRAPH_DELETE_PTR(last);
    aspect_arr_.pop_back();    // 弹出最后一个
    DAOGRAPH_FUNCTION_END
}


CVoidPtr GAspectManager::setGParamManager(GParamManagerPtr pm) {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(pm)
    for (auto *cur: aspect_arr_) {
        cur->setGParamManager(pm);
    }
    return this;
}


CVoidPtr GAspectManager::setGEventManager(GEventManagerPtr em) {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(em)
    for (auto *cur : aspect_arr_) {
        cur->setGEventManager(em);
    }
    return this;
}

CVoidPtr GAspectManager::setBelong(GElement* belong) {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(belong)
    for (auto *cur : aspect_arr_) {
        cur->belong_ = belong;
    }
    return this;
}

DAOGRAPH_NAMESPACE_END
