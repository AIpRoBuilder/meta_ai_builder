/***************************

@File: GSome.inl
@Time: 2023/8/20 11:20
@Desc:
***************************/

#ifndef DAOGRAPH_GSOME_INL
#define DAOGRAPH_GSOME_INL

#include "GSome.h"

DAOGRAPH_NAMESPACE_BEGIN

template<CInt TriggerNum>
GSome<TriggerNum>::GSome() {
    element_type_ = GElementType::SOME;
    session_ = URandom<>::generateSession(DAOGRAPH_STR_SOME);
}


template<CInt TriggerNum>
CStatus GSome<TriggerNum>::addElementEx(GElementPtr element) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)
    DAOGRAPH_ASSERT_NOT_NULL(element)

    /**
     * GSome中插入的元素，必须要有 timeout的设定，如果没有的话，强行写到max值。
     * 用于确保在pipeline run执行完成之前，所有的 element 都会被回收
     */
    if (DAOGRAPH_DEFAULT_ELEMENT_TIMEOUT == element->timeout_) {
        element->setTimeout(DAOGRAPH_MAX_BLOCK_TTL, GElementTimeoutStrategy::HOLD_BY_PIPELINE);
    }

    DAOGRAPH_FUNCTION_END
}


template<CInt TriggerNum>
CStatus GSome<TriggerNum>::run()  {
    DAOGRAPH_FUNCTION_BEGIN

    left_num_ = TriggerNum;    // 还剩n个，就完成当前GSome的执行逻辑
    cur_status_ = CStatus();

    /**
     * 1. 并发的执行，其中的所有逻辑信息
     * 2. 等待 Trigger Num 个element执行完成
     * 3. 将所有未执行完的element 设置为timeout
     * 4. 赋返回值
     */
    for (auto* element : group_elements_arr_) {
        thread_pool_->commit([this, element] {
            auto curStatus = element->fatProcessor(CFunctionType::RUN);
            {
                DAOGRAPH_UNIQUE_LOCK lock(lock_);
                cur_status_ += curStatus;
                left_num_--;
            }
            cv_.notify_one();
        }, DAOGRAPH_POOL_TASK_STRATEGY);
    }

    DAOGRAPH_UNIQUE_LOCK lock(lock_);
    cv_.wait(lock, [this] {
        return left_num_ <= 0 || cur_status_.isErr();
    });

    for (GElementPtr element : group_elements_arr_) {
        if (!element->done_) {
            element->cur_state_.store(GElementState::TIMEOUT, std::memory_order_release);
        }
    }
    status = cur_status_;
    DAOGRAPH_FUNCTION_END
}


template<CInt TriggerNum>
CBool GSome<TriggerNum>::isSerializable() const {
    return false;    // 情况较为复杂，默认不可以
}


template<CInt TriggerNum>
CVoid GSome<TriggerNum>::dump(std::ostream& oss) {
    dumpElement(oss);
    dumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;style=dashed;\n";    // 蓝色虚线

    for (const auto& element : group_elements_arr_) {
        element->dump(oss);
    }

    dumpGroupLabelEnd(oss);

    for (const auto& element : run_before_) {
        dumpEdge(oss, this, element);
    }
}


template<CInt TriggerNum>
CBool GSome<TriggerNum>::isHold() {
    // 这里固定是不可以 hold的
    return false;
}


template<CInt TriggerNum>
CStatus GSome<TriggerNum>::checkSuitable() {
    DAOGRAPH_FUNCTION_BEGIN
    status = GElement::checkSuitable();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((DAOGRAPH_DEFAULT_LOOP_TIMES != loop_), "GSome cannot set loop > 1.")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((0 >= TriggerNum), "trigger num must bigger than 0.")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((group_elements_arr_.size() < TriggerNum),     \
                                            "this GSome need at least [" + std::to_string(TriggerNum) + "] element.")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(std::any_of(group_elements_arr_.begin(), group_elements_arr_.end(), [](GElementPtr ptr) {
        return !ptr->isAsync();
    }), "GSome contains async node only.")

    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GSOME_INL