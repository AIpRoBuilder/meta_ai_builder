/***************************

@File: GMultiCondition.cpp
@Time: 2023/6/17 19:21
@Desc: 
***************************/

#ifndef DAOGRAPH_GMULTICONDITION_INL
#define DAOGRAPH_GMULTICONDITION_INL

#include "GMultiCondition.h"

DAOGRAPH_NAMESPACE_BEGIN

template<GMultiConditionType type>
GMultiCondition<type>::GMultiCondition() {
    element_type_ = GElementType::MULTI_CONDITION;
    session_ = URandom<>::generateSession(DAOGRAPH_STR_MULTI_CONDITION);
}


template<GMultiConditionType type>
CStatus GMultiCondition<type>::run() {
    DAOGRAPH_FUNCTION_BEGIN

    switch (type) {
        case GMultiConditionType::SERIAL:
            status = serialRun();
            break;
        case GMultiConditionType::PARALLEL:
            status = parallelRun();
            break;
        default:
            DAOGRAPH_RETURN_ERROR_STATUS("unknown multi condition type")
    }

    DAOGRAPH_FUNCTION_END
}


template<GMultiConditionType type>
CStatus GMultiCondition<type>::serialRun() {
    DAOGRAPH_FUNCTION_BEGIN

    for (auto cur: this->group_elements_arr_) {
        if (cur->isMatch()) {
            // 仅依次执行 match 的逻辑
            status = cur->fatProcessor(CFunctionType::RUN);
            DAOGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    DAOGRAPH_FUNCTION_END
}


template<GMultiConditionType type>
CStatus GMultiCondition<type>::parallelRun() {
    DAOGRAPH_FUNCTION_BEGIN
    std::vector<std::future<CStatus> > futures;
    for (GElementPtr cur : this->group_elements_arr_) {
        if (!cur->isMatch()) {
            continue;    // 不满足条件，则不执行
        }

        futures.emplace_back(std::move(this->thread_pool_->commit([cur] {
            return cur->fatProcessor(CFunctionType::RUN);
        }, cur->binding_index_)));
    }

    for (auto& fut: futures) {
        /**
         * 如果有异常值的化，也等待所有内容计算完成后，统一返回
         * 暂时没有处理超时的情况。预计今后会统一处理
         */
        status += fut.get();
    }

    DAOGRAPH_FUNCTION_END
}


template<GMultiConditionType type>
CIndex GMultiCondition<type>::choose() {
    // GMultiCondition 是不可能执行到 choose()方法的。这里直接抛出异常好了
    DAOGRAPH_THROW_EXCEPTION("GMultiCondition no need choose function")
    return 0;
}


template<GMultiConditionType type>
CBool GMultiCondition<type>::isSerializable() const {
    if (GMultiConditionType::PARALLEL == type && group_elements_arr_.size() > 1) {
        /**
         * 如果是PARALLEL模式的话，并且其中的元素个数大于1，则一定不可以串行执行
         * PARALLEL模式中，仅有一个元素的情况，和 SERIAL模式的判断方式一样，
         * 均为判断其中所有的element是否可以并行
         * 故放在下面的条件中判断了
         */
        return false;
    }

    return GGroup::isSerializable();
}


template<GMultiConditionType type>
CBool GMultiCondition<type>::isSeparate(GElementCPtr a, GElementCPtr b) const {
    CBool result = false;
    switch (type) {
        case GMultiConditionType::SERIAL:
            result = true;
            break;
        case GMultiConditionType::PARALLEL:
            result = false;
            break;
        default:
            DAOGRAPH_THROW_EXCEPTION("unknown multi condition type in isSeparateWith function")
    }
    return result;
}

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GMULTICONDITION_INL
