/***************************

@File: MyTemplateNode.h
@Time: 2021/9/18 9:52 下午
@Desc:
***************************/

#ifndef DAOGRAPH_MYTEMPLATENODE_H
#define DAOGRAPH_MYTEMPLATENODE_H

#include "DaoGraph.h"

template <typename ...Args>
class MyTemplateNode : public DaoGraph::GTemplateNode<Args ...> {
public:
    // 通过两种不同的方式，实现构造函数，可以在pipeline中通过不同的方式 register 进来
    explicit MyTemplateNode(int num, float score) {
        num_ = num;
        score_ = score;
    }

    explicit MyTemplateNode(int num) {
        num_ = num;
        score_ = 7.0f;    // 默认值
    }

    CStatus run () override {
        DaoGraph::DAOGRAPH_ECHO("[MyTemplateNode] num = [%d], score = [%f]", num_, score_);
        return CStatus();
    }

private:
    int num_;
    float score_;
};

#endif //DAOGRAPH_MYTEMPLATENODE_H
