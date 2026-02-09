/***************************

@File: MyStageNode.h
@Time: 2024/12/15 18:46
@Desc: 
***************************/

#ifndef DAOGRAPH_MYSTAGENODE_H
#define DAOGRAPH_MYSTAGENODE_H

#include "DaoGraph.h"

const static char* kStageKey = "stage";

template<int before, int after>
class MyStageNode : public DaoGraph::GNode {
    CStatus run() override {
        DAOGRAPH_SLEEP_SECOND(before);
        DaoGraph::DAOGRAPH_ECHO("[%s] wait for stage", getName().c_str());

        // 等到所有的流程，都完成等待之后，会统一往下执行
        enterStage(kStageKey);
        DaoGraph::DAOGRAPH_ECHO("[%s] finish stage, before [%d]s, after [%d]s", getName().c_str(), before, after);
        DAOGRAPH_SLEEP_SECOND(after);
        return CStatus();
    }
};

#endif //DAOGRAPH_MYSTAGENODE_H
