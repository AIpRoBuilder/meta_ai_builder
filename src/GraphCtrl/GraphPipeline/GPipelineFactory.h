/***************************

@File: GPipelineFactory.h
@Time: 2021/6/23 10:25 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GPIPELINEFACTORY_H
#define DAOGRAPH_GPIPELINEFACTORY_H

#include <mutex>
#include <list>
#include <map>

#include "GPipeline.h"
#include "GPipelineObject.h"

DAOGRAPH_NAMESPACE_BEGIN

class GPipelineFactory : public GPipelineObject {
public:
    /**
     * 创建一个pipeline信息
     * @return
     */
    static GPipelinePtr create();

    /**
     * 销毁一个pipeline信息
     * @param pipeline
     * @return
     */
    static CStatus remove(GPipelinePtr pipeline);

    /**
     * 清空所有的pipeline信息
     */
    static CStatus clear();

private:
    static GPipelinePtrList s_pipeline_list_;    // 记录所有的 pipeline 信息
    static std::mutex s_lock_;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GPIPELINEFACTORY_H
