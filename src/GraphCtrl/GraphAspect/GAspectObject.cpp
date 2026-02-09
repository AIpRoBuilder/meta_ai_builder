/***************************

@File: GAspectObject.cpp
@Time: 2024/4/1 21:53
@Desc: 
***************************/

#include "GAspectObject.h"
#include "../GraphElement/GElementInclude.h"

DAOGRAPH_NAMESPACE_BEGIN

GAspectObject::GAspectObject() {
    session_ = URandom<>::generateSession(DAOGRAPH_STR_ASPECT);
}


const std::string& GAspectObject::getName() const {
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(belong_);
    return belong_->getName();
}


GAspectObject::~GAspectObject() {
    DAOGRAPH_DELETE_PTR(param_)
}

DAOGRAPH_NAMESPACE_END