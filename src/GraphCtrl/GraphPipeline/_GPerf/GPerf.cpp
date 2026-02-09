/***************************

@File: GPerf.cpp
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#include "GPerf.h"
#include "GPerfDefine.h"
#include "../GPipeline.h"
#include "../../GraphElement/_GOptimizer/GOptimizerInclude.h"

DAOGRAPH_NAMESPACE_BEGIN

CStatus GPerf::perf(GPipelinePtr pipeline, std::ostream& oss) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(pipeline)

    status = inject(pipeline);
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->process();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = markLongestPath(pipeline);
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = pipeline->dump(oss);
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = recover(pipeline);
    DAOGRAPH_FUNCTION_CHECK_STATUS

    DAOGRAPH_FUNCTION_END
}


CStatus GPerf::inject(GPipelinePtr pipeline) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(pipeline)

    const CFMSec now = DAOGRAPH_GET_CURRENT_ACCURATE_MS();
    for (auto* cur : pipeline->repository_.elements_) {
        /**
         * 给其中的每个element，都添加这个切面信息
         * 这里是不需要考虑 delete perf_info_的，因为在 element结束的时候，会自动释放
         */
        cur->perf_info_ = CAllocator::safeMallocCStruct<GPerfInfo>();
        cur->addGAspect<GPerfAspect<CFMSec, GPerfInfoPtr>>(now, cur->perf_info_);
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GPerf::markLongestPath(GPipelinePtr pipeline) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(pipeline, pipeline->element_manager_)
    GElementPtrArr longestPath;
    CFMSec maxTs = 0.0;
    auto paths = GOptimizer::collectPaths(pipeline->element_manager_->manager_elements_);
    for (const auto& path : paths) {
        CFMSec curTs = 0.0;
        for (auto* element : path) {
            DAOGRAPH_ASSERT_NOT_NULL(element)
            curTs += element->perf_info_->accu_cost_ts_;
        }
        if (curTs >= maxTs) {
            maxTs = curTs;
            longestPath = path;
        }
    }

    for (auto* element : longestPath) {
        DAOGRAPH_ASSERT_NOT_NULL(element->perf_info_)
        element->perf_info_->in_longest_path_ = true;
    }
    DAOGRAPH_FUNCTION_END
}


CStatus GPerf::recover(GPipelinePtr pipeline) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(pipeline)
    for (auto* cur : pipeline->repository_.elements_) {
        status += cur->popLastAspect();
        DAOGRAPH_DELETE_PTR(cur->perf_info_)
    }
    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END
