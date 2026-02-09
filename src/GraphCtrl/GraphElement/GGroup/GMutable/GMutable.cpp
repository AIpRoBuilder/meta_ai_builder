/***************************

@File: GMutable.cpp
@Time: 2023/11/13 22:21
@Desc: 
***************************/

#include "GMutable.h"

DAOGRAPH_NAMESPACE_BEGIN

GMutable::GMutable() {
    element_type_ = GElementType::MUTABLE;
    session_ = URandom<>::generateSession(DAOGRAPH_STR_MUTABLE);
    manager_ = DAOGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
}


GMutable::~GMutable() {
    DAOGRAPH_DELETE_PTR(manager_)
}


CStatus GMutable::init() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(manager_)
    status = manager_->initEngine();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = GGroup::init();
    DAOGRAPH_FUNCTION_END
}


CStatus GMutable::run() {
    DAOGRAPH_FUNCTION_BEGIN

    /**
     * 1. 初始化内容
     * 2. 通过外部复写 reshape()，来实现关系设定。其中，通过 --> 设定的，是会自动恢复visible的
     * 3. 通过 manager 执行
     */
    setup();
    status = reshape(group_elements_arr_);
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = manager_->process({group_elements_arr_.begin(), group_elements_arr_.end()});
    DAOGRAPH_FUNCTION_END
}


CStatus GMutable::destroy() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto* element : group_elements_arr_) {
        // 链路中，可能会将部分内容设置为 visible 的信息。这里统一恢复一下
        element->setVisible(true);
    }

    status = GGroup::destroy();
    DAOGRAPH_FUNCTION_END
}


CBool GMutable::isSerializable() const {
    // 情况随时可能发生变化，直接设定不可以 serialize 即可
    return false;
}


GElementPtr GMutable::setThreadPoolEx(UThreadPoolPtr ptr) {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(manager_, ptr)
    manager_->setThreadPool(ptr);
    return this;
}


CVoid GMutable::setup() {
    for (auto* element : group_elements_arr_) {
        element->run_before_.clear();
        element->dependence_.clear();
        element->setLoop(1);
        element->setVisible(false);
    }
}

DAOGRAPH_NAMESPACE_END
