/***************************

@File: MyRecvMessageNode.h
@Time: 2022/12/27 00:11
@Desc: 
***************************/

#ifndef DAOGRAPH_MYRECVMESSAGENODE_H
#define DAOGRAPH_MYRECVMESSAGENODE_H

#include "DaoGraph.h"
#include "../MyParams/MyMessageParam.h"

class MyRecvMessageNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        std::shared_ptr<MyMessageParam> mp = nullptr;    // 接收一个消息
        CStatus status = DAOGRAPH_RECV_MPARAM(MyMessageParam, "send-recv", mp);
        if (!status.isOK()) {
            DaoGraph::DAOGRAPH_ECHO("MySubMessageNode sub message error");
            return status;
        }

        DaoGraph::DAOGRAPH_ECHO("num = [%d], info = [%s]", mp->num, mp->info.c_str());
        return status;
    }
};

#endif //DAOGRAPH_MYRECVMESSAGENODE_H
