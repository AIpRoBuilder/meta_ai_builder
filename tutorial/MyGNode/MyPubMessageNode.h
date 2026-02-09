/***************************

@File: MyPubMessageNode.h
@Time: 2022/11/4 00:02
@Desc: 
***************************/

#ifndef DAOGRAPH_MYPUBMESSAGENODE_H
#define DAOGRAPH_MYPUBMESSAGENODE_H

#include "DaoGraph.h"
#include "../MyParams/MyMessageParam.h"

class MyPubMessageNode : public DaoGraph::GNode {
public:
    CStatus init() override {
        int sleep = 300;
        DAOGRAPH_SLEEP_MILLISECOND(sleep)    // 休息，确保其他的sub节点，已经完成了订阅
        DaoGraph::DAOGRAPH_ECHO("==> [MyPubMessageNode] [init], sleep [%d] ms, to make sure sub node prepare ok.",
                            sleep);
        return CStatus();
    }

    CStatus run() override {
        std::shared_ptr<MyMessageParam> mp(new MyMessageParam());    // 创建一个消息，并且发送出去
        mp->num = (num_++) * 100;
        mp->info = "this is a test info, num = " + std::to_string(mp->num);
        CStatus status = DAOGRAPH_PUB_MPARAM(MyMessageParam, "pub-sub", mp, DaoGraph::GMessagePushStrategy::WAIT);
        return status;
    }

private:
    int num_ = 0;
};


#endif //DAOGRAPH_MYPUBMESSAGENODE_H
