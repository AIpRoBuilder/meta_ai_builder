/***************************

@File: GPerf.h
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#ifndef DAOGRAPH_GPERF_H
#define DAOGRAPH_GPERF_H

#include "../../GraphObject.h"
#include "GPerfAspect.h"
#include "GPerfDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

class GPipeline;

class GPerf : public GraphObject {
protected:
    /**
     * 针对pipeline进行性能分析
     * @param pipeline
     * @return
     */
    static CStatus perf(GPipeline* pipeline, std::ostream& oss);

    /**
     * 注入对应的切面信息
     * @param pipeline
     * @return
     */
    static CStatus inject(GPipeline* pipeline);

    /**
     * 查询最长链路信息
     * @param pipeline
     * @return
     */
    static CStatus markLongestPath(GPipeline* pipeline);

    /**
     * 恢复原来的pipeline信息
     * @param pipeline
     * @return
     */
    static CStatus recover(GPipeline* pipeline);

    friend class GPipeline;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GPERF_H
