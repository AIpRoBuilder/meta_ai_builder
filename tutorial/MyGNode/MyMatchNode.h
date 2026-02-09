/***************************

@File: MyMatchNode.h
@Time: 2023/8/18 22:00
@Desc: 
***************************/

#ifndef DAOGRAPH_MYMATCHNODE_H
#define DAOGRAPH_MYMATCHNODE_H

#include "DaoGraph.h"
#include "../MyParams/MyParam.h"

class MyMatchNode : public DaoGraph::GNode {
public:
    CStatus run () override {
        auto* myParam = DAOGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1");
        int value = 0;
        {
            DAOGRAPH_PARAM_READ_CODE_BLOCK(myParam)
            value = myParam->iValue;
        }
        DaoGraph::DAOGRAPH_ECHO("[%s], iValue is [%d], Sleep for [%d] second ... ",    \
                            this->getName().c_str(), value, value);
        DAOGRAPH_SLEEP_SECOND(value)
        return CStatus();
    }

    CBool isMatch() override {
        /**
         * 本函数默认返回 false，所有的 GElement 即在 MultiCondition 中默认不执行
         * 如果需要被执行，则复写本函数，并且在调价符合的时候，返回 true，即可执行
         */
        auto* myParam = DAOGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1");
        return 0 != myParam->iCount % 2;    // 当本条件返回是 true的时候，本节点才会在 MultiCondition 中被执行
    }
};

#endif //DAOGRAPH_MYMATCHNODE_H
