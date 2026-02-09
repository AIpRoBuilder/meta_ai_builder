/***************************

@File: PywGPipelineDeleter.h
@Time: 2025/4/19 12:37
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWGPIPELINEDELETER_H
#define DAOGRAPH_PYWGPIPELINEDELETER_H

#include "DaoGraph.h"

struct PywGPipelineDeleter {
    void operator()(DaoGraph::GPipelinePtr ptr) {
        DaoGraph::GPipelineFactory::remove(ptr);
    }
};

#endif //DAOGRAPH_PYWGPIPELINEDELETER_H
