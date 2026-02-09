/***************************

@File: GPipelineFactory.cpp
@Time: 2021/6/23 10:25 下午
@Desc: 
***************************/

#include "GPipelineFactory.h"

DAOGRAPH_NAMESPACE_BEGIN

GPipelinePtrList GPipelineFactory::s_pipeline_list_ {};
std::mutex GPipelineFactory::s_lock_ {};

GPipelinePtr GPipelineFactory::create() {
    DAOGRAPH_FUNCTION_BEGIN
    auto pipeline = DAOGRAPH_SAFE_MALLOC_COBJECT(GPipeline)
    {
        DAOGRAPH_LOCK_GUARD lock(s_lock_);
        s_pipeline_list_.emplace_back(pipeline);
    }
    return pipeline;
}


CStatus GPipelineFactory::remove(GPipelinePtr pipeline) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(pipeline)

    {
        DAOGRAPH_LOCK_GUARD lock(s_lock_);
        s_pipeline_list_.remove(pipeline);
    }
    DAOGRAPH_DELETE_PTR(pipeline)
    DAOGRAPH_FUNCTION_END
}


CStatus GPipelineFactory::clear() {
    DAOGRAPH_FUNCTION_BEGIN
    {
        DAOGRAPH_LOCK_GUARD lock(s_lock_);
        for (GPipelinePtr pipeline : GPipelineFactory::s_pipeline_list_) {
            DAOGRAPH_DELETE_PTR(pipeline)
        }
        s_pipeline_list_.clear();
    }
    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END
