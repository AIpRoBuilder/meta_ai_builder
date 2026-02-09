/***************************

@File: GMessage.h
@Time: 2022/10/30 20:38
@Desc: 
***************************/

#ifndef DAOGRAPH_GMESSAGE_H
#define DAOGRAPH_GMESSAGE_H

#include <memory>

#include "GMessageObject.h"
#include "GMessageDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

template<typename T, CUInt capacity = DAOGRAPH_DEFAULT_RINGBUFFER_SIZE,
        c_enable_if_t<std::is_base_of<GMessageParam, T>::value, int> = 0>
class GMessage : public GMessageObject {
public:
    explicit GMessage(CUInt size = capacity) {
        queue_.setCapacity(size);
    }

    /**
     * 析构函数。释放前，要先释放队列中所有的信息
     */
    ~GMessage() override {
        queue_.clear();
    }

    /**
     * 写入智能指针类型的参数
     * @tparam TImpl
     * @param value
     * @param strategy
     * @return
     */
    template<class TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CVoid send(const std::shared_ptr<TImpl>& value, GMessagePushStrategy strategy) {
        queue_.push(value, strategy);
    }

    /**
     * 通过智能指针的方式传递
     * @tparam TImpl
     * @param value
     * @param timeout
     * @return
     */
    template<class TImpl,
            c_enable_if_t<std::is_base_of<T, TImpl>::value, int> = 0>
    CStatus recv(std::shared_ptr<TImpl>& value, CMSec timeout) {
        return queue_.waitPopWithTimeout(value, timeout);
    }

    /**
     * 获取容量大小
     * @return
     */
    CUInt getCapacity() const {
        return queue_.getCapacity();
    }

private:
    UAtomicRingBufferQueue<T, capacity> queue_;
};


template<typename T, CUInt capacity = DAOGRAPH_DEFAULT_RINGBUFFER_SIZE>
using GMessagePtr = GMessage<T, capacity> *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GMESSAGE_H
