/***************************

@File: GSchedule.cpp
@Time: 2023/3/25 19:01
@Desc: 
***************************/

#include "GSchedule.h"

DAOGRAPH_NAMESPACE_BEGIN

CStatus GSchedule::init() {
    DAOGRAPH_FUNCTION_BEGIN

    if (internal::GScheduleType::UNIQUE == type_) {
        /**
         * 如果是 unique 的数据，则在这里初始化一下
         * 如果是 shared 的数据，则不做任何操作
         */
        DAOGRAPH_DELETE_PTR(unique_tp_)
        unique_tp_ = CAllocator::safeMallocTemplateCObject<UThreadPool>(true, config_);
    } else if (internal::GScheduleType::SHARED == type_) {
        DAOGRAPH_ASSERT_NOT_NULL(shared_tp_)
        // 首先，要确定外部传入的线程池，已经初始化过了
        if (!shared_tp_->isInit()) {
            DAOGRAPH_RETURN_ERROR_STATUS("shared thread pool is not init.")
        }
    }

    DAOGRAPH_FUNCTION_END
}


CStatus GSchedule::destroy() {
    DAOGRAPH_FUNCTION_BEGIN

    if (internal::GScheduleType::UNIQUE == type_) {
        // unique 模式下，init的时候创建线程，就在destroy的时候，给析构掉了
        DAOGRAPH_DELETE_PTR(this->unique_tp_)
    }

    DAOGRAPH_FUNCTION_END
}


CStatus GSchedule::run() {
    DAOGRAPH_NO_SUPPORT
}


CStatus GSchedule::makeType(UThreadPoolPtr tp) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_DELETE_PTR(this->unique_tp_)
    shared_tp_ = tp;    // 用外部传入的线程池，来代替内部原有的线程池

    // 如果是nullptr的话，就当做是恢复使用默认本地线程池
    type_ = (nullptr == tp) ? internal::GScheduleType::UNIQUE : internal::GScheduleType::SHARED;
    DAOGRAPH_FUNCTION_END
}


UThreadPoolPtr GSchedule::getThreadPool() {
    /**
     * 返回一个可用的 pool
     * utp和stp，应该是有且仅有一个为非空的
     */
    return unique_tp_ == nullptr ? shared_tp_ : unique_tp_;
}


GSchedule::~GSchedule() {
    destroy();
}

DAOGRAPH_NAMESPACE_END
