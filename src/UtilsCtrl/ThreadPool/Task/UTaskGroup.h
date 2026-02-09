/***************************

@File: UTaskGroup.h
@Time: 2022/1/2 2:17 下午
@Desc: 任务组，用于批量提交
***************************/

#ifndef DAOGRAPH_UTASKGROUP_H
#define DAOGRAPH_UTASKGROUP_H

#include <utility>

#include "../UThreadObject.h"

DAOGRAPH_NAMESPACE_BEGIN

class UTaskGroup : public UThreadObject {
public:
    explicit UTaskGroup() = default;
    DAOGRAPH_NO_ALLOWED_COPY(UTaskGroup)

    /**
     * 直接通过函数来申明taskGroup
     * @param task
     * @param ttl
     * @param onFinished
     */
    explicit UTaskGroup(DAOGRAPH_DEFAULT_CONST_FUNCTION_REF task,
                        CMSec ttl = DAOGRAPH_MAX_BLOCK_TTL,
                        DAOGRAPH_CALLBACK_CONST_FUNCTION_REF onFinished = nullptr) noexcept {
        this->addTask(task)
            ->setTtl(ttl)
            ->setOnFinished(onFinished);
    }

    /**
     * 添加一个任务
     * @param task
     */
    UTaskGroup* addTask(DAOGRAPH_DEFAULT_CONST_FUNCTION_REF task) {
        task_arr_.emplace_back(task);
        return this;
    }

    /**
     * 设置任务最大超时时间
     * @param ttl
     */
    UTaskGroup* setTtl(CMSec ttl) {
        this->ttl_ = ttl;
        return this;
    }

    /**
     * 设置执行完成后的回调函数
     * @param onFinished
     * @return
     */
    UTaskGroup* setOnFinished(DAOGRAPH_CALLBACK_CONST_FUNCTION_REF onFinished) {
        this->on_finished_ = onFinished;
        return this;
    }

    /**
     * 获取最大超时时间信息
     * @return
     */
    CMSec getTtl() const {
        return this->ttl_;
    }

    /**
     * 清空任务组
     */
    CVoid clear() {
        task_arr_.clear();
    }

    /**
     * 获取任务组大小
     * @return
     */
    CSize getSize() const {
        auto size = task_arr_.size();
        return size;
    }

private:
    std::vector<DAOGRAPH_DEFAULT_FUNCTION> task_arr_;         // 任务消息
    CMSec ttl_ = DAOGRAPH_MAX_BLOCK_TTL;                      // 任务组最大执行耗时(如果是0的话，则表示不阻塞)
    DAOGRAPH_CALLBACK_FUNCTION on_finished_ = nullptr;        // 执行函数任务结束

    friend class UThreadPool;
};

using UTaskGroupPtr = UTaskGroup *;
using UTaskGroupRef = UTaskGroup &;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UTASKGROUP_H
