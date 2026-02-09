/***************************

@File: UThreadPoolConfig.h
@Time: 2022/1/3 9:31 下午
@Desc: 线程池配置信息
***************************/

#ifndef DAOGRAPH_UTHREADPOOLCONFIG_H
#define DAOGRAPH_UTHREADPOOLCONFIG_H

#include <algorithm>

#include "UThreadObject.h"
#include "UThreadPoolDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

struct UThreadPoolConfig : public CStruct {
    /** 具体值含义，参考UThreadPoolDefine.h文件 */
    CInt default_thread_size_ = DAOGRAPH_DEFAULT_THREAD_SIZE;
    CInt secondary_thread_size_ = DAOGRAPH_SECONDARY_THREAD_SIZE;
    CInt max_thread_size_ = DAOGRAPH_MAX_THREAD_SIZE;
    CInt max_task_steal_range_ = DAOGRAPH_MAX_TASK_STEAL_RANGE;
    CInt max_local_batch_size_ = DAOGRAPH_MAX_LOCAL_BATCH_SIZE;
    CInt max_pool_batch_size_ = DAOGRAPH_MAX_POOL_BATCH_SIZE;
    CInt max_steal_batch_size_ = DAOGRAPH_MAX_STEAL_BATCH_SIZE;
    CInt primary_thread_busy_epoch_ = DAOGRAPH_PRIMARY_THREAD_BUSY_EPOCH;
    CMSec primary_thread_empty_interval_ = DAOGRAPH_PRIMARY_THREAD_EMPTY_INTERVAL;
    CSec secondary_thread_ttl_ = DAOGRAPH_SECONDARY_THREAD_TTL;
    CSec monitor_span_ = DAOGRAPH_MONITOR_SPAN;
    CMSec queue_emtpy_interval_ = DAOGRAPH_QUEUE_EMPTY_INTERVAL;
    CInt primary_thread_policy_ = DAOGRAPH_PRIMARY_THREAD_POLICY;
    CInt secondary_thread_policy_ = DAOGRAPH_SECONDARY_THREAD_POLICY;
    CInt primary_thread_priority_ = DAOGRAPH_PRIMARY_THREAD_PRIORITY;
    CInt secondary_thread_priority_ = DAOGRAPH_SECONDARY_THREAD_PRIORITY;
    CBool bind_cpu_enable_ = DAOGRAPH_BIND_CPU_ENABLE;
    CBool batch_task_enable_ = DAOGRAPH_BATCH_TASK_ENABLE;
    CBool monitor_enable_ = DAOGRAPH_MONITOR_ENABLE;

    CStatus check() const {
        DAOGRAPH_FUNCTION_BEGIN
        if (default_thread_size_ < 0 || secondary_thread_size_ < 0) {
            DAOGRAPH_RETURN_ERROR_STATUS("thread size cannot less than 0")
        }

        if (default_thread_size_ + secondary_thread_size_ > max_thread_size_) {
            DAOGRAPH_RETURN_ERROR_STATUS("max thread size is less than default + secondary thread. ["     \
            + std::to_string(max_thread_size_) + "<" + std::to_string(default_thread_size_) + "+"    \
            + std::to_string(secondary_thread_size_)  + "]");
        }

        if (monitor_enable_ && monitor_span_ <= 0) {
            DAOGRAPH_RETURN_ERROR_STATUS("monitor span cannot less than 0")
        }
        DAOGRAPH_FUNCTION_END
    }

protected:
    /**
     * 计算可盗取的范围，盗取范围不能超过默认线程数-1
     * @return
     */
    int calcStealRange() const {
        int range = (std::min)(this->max_task_steal_range_, this->default_thread_size_ - 1);
        return range;
    }

    friend class UThreadPrimary;
    friend class UThreadSecondary;
};

using UThreadPoolConfigPtr = UThreadPoolConfig *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UTHREADPOOLCONFIG_H
