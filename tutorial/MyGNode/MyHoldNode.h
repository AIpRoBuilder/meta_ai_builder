/***************************

@File: MyHoldNode.h
@Time: 2022/4/6 21:19
@Desc: 
***************************/

#ifndef DAOGRAPH_MYHOLDNODE_H
#define DAOGRAPH_MYHOLDNODE_H

#include "DaoGraph.h"
#include "../MyParams/MyParam.h"

static const char* HOLD_PARAM_NAME = "hold-param";

class MyHoldNode : public DaoGraph::GNode {
public:
    CStatus init() override {
        CStatus status = DAOGRAPH_CREATE_GPARAM(MyParam, HOLD_PARAM_NAME)
        return status;
    }

    CStatus run() override {
        auto param = DAOGRAPH_GET_GPARAM(MyParam, HOLD_PARAM_NAME)
        DAOGRAPH_ASSERT_NOT_NULL(param)

        param->iValue++;
        DaoGraph::DAOGRAPH_ECHO("iCount value is [%d]", param->iValue);
        return CStatus();
    }

    CBool isHold() override {
        /**
         * 针对当前场景，添加 hold逻辑
         * 当 hold逻辑被满足的时候，会重复执行当前节点，直到返回false为止
         * 也可以用isHold机制，实现算子内部自己的状态机机制
         */
        auto param = DAOGRAPH_GET_GPARAM(MyParam, HOLD_PARAM_NAME)
        if (nullptr == param) {
            return false;    // 如果未读取到参数，则直接结束，不继续执行了
        }

        DaoGraph::DAOGRAPH_ECHO("enter hold path, iValue = [%d]", param->iValue);
        return param->iValue < 5;    // 当 iValue 值小于5的时候，此节点会持续执行
    }
};

#endif //DAOGRAPH_MYHOLDNODE_H
