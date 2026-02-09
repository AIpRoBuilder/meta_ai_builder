/***************************

@File: MyReadParamNode.h
@Time: 2021/6/13 3:23 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYREADPARAMNODE_H
#define DAOGRAPH_MYREADPARAMNODE_H

#include "DaoGraph.h"
#include "../MyParams/MyParam.h"

class MyReadParamNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        auto myParam = DAOGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")    // 获取类型为MyParam且名为"param1"的参数
        int val = 0;
        {
            /* 对需要使用（读或写）参数的位置，加括号{}范围限定，以减少互斥等待时间 */
            DAOGRAPH_PARAM_READ_CODE_BLOCK(myParam)
            val = myParam->iValue;
        }
        DaoGraph::DAOGRAPH_ECHO("[%s], iValue is : [%d] ... ", this->getName().c_str(), val);

        return CStatus();
    }
};

#endif //DAOGRAPH_MYREADPARAMNODE_H
