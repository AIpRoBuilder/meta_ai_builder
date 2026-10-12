/***************************

@File: GNode.cpp
@Time: 2022/3/28 23:14
@Desc: 
***************************/

#include "GNode.h"

DAOGRAPH_NAMESPACE_BEGIN

GNode::GNode() : GElement() {
    element_type_ = GElementType::NODE;
    session_ = URandom<>::generateSession(DAOGRAPH_STR_NODE);
}


GNodePtr GNode::setType(const GNodeType& type) {
    node_type_ = type;
    return this;
}


GNodePtr GNode::setWaitFunc(const GWaitFunc& func) {
    GElement::setWaitFunc(func);
    return this;
}


CStatus GNode::spawn(const UTaskGroup& tasks, CMSec ttl) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(true)
    DAOGRAPH_ASSERT_NOT_NULL(thread_pool_)

    status = thread_pool_->submit(tasks, ttl);
    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END
