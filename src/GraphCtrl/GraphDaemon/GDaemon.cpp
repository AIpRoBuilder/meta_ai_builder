/***************************

@File: GDaemon.cpp
@Time: 2022/2/3 1:19 下午
@Desc: 
***************************/

#include "GDaemon.h"

DAOGRAPH_NAMESPACE_BEGIN

CStatus GDaemon::init() {
    DAOGRAPH_FUNCTION_BEGIN
    timer_.start(interval_, [this] {
        daemonTask(param_);
    }, [this] {
        return modify(param_);
    });
    DAOGRAPH_FUNCTION_END
}


CStatus GDaemon::destroy() {
    DAOGRAPH_FUNCTION_BEGIN
    timer_.stop();
    DAOGRAPH_FUNCTION_END
}


CMSec GDaemon::getInterval() const {
    return interval_;
}


CMSec GDaemon::modify(GDaemonParamPtr param) {
    return 0;
}


CMSec GDaemon::__getInterval_4py() const {
    return getInterval();
}

DAOGRAPH_NAMESPACE_END
