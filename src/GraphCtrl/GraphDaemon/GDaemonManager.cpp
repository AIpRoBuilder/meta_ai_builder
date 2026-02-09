/***************************

@File: GDaemonManager.cpp
@Time: 2022/2/3 10:26 上午
@Desc: 
***************************/

#include "GDaemonManager.h"

DAOGRAPH_NAMESPACE_BEGIN

GDaemonManager::~GDaemonManager() {
    clear();
}


CStatus GDaemonManager::init() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto daemon: daemons_) {
        DAOGRAPH_ASSERT_NOT_NULL(daemon)
        status += daemon->init();
    }

    DAOGRAPH_FUNCTION_END
}


CStatus GDaemonManager::destroy() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto daemon: daemons_) {
        DAOGRAPH_ASSERT_NOT_NULL(daemon)
        status += daemon->destroy();
    }

    DAOGRAPH_FUNCTION_END
}


CStatus GDaemonManager::add(GDaemonPtr daemon) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(daemon)

    daemons_.insert(daemon);
    DAOGRAPH_FUNCTION_END
}


CStatus GDaemonManager::remove(GDaemonPtr daemon) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(daemon)

    daemons_.erase(daemon);
    DAOGRAPH_DELETE_PTR(daemon)
    DAOGRAPH_FUNCTION_END
}


CStatus GDaemonManager::clear() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto daemon: daemons_) {
        DAOGRAPH_DELETE_PTR(daemon)
    }

    daemons_.clear();
    DAOGRAPH_FUNCTION_END
}


CSize GDaemonManager::getSize() const {
    return daemons_.size();
}

DAOGRAPH_NAMESPACE_END
