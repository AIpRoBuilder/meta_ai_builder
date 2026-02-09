/***************************

@File: MySubMessageNode.h
@Time: 2022/11/4 00:02
@Desc: 
***************************/

#ifndef DAOGRAPH_MYSUBMESSAGENODE_H
#define DAOGRAPH_MYSUBMESSAGENODE_H

#include "DaoGraph.h"
#include "../MyParams/MyMessageParam.h"

template <typename ...Args>
class MySubMessageNode : public DaoGraph::GTemplateNode<Args ...> {
public:
    explicit MySubMessageNode(int connId) {
        conn_id_ = connId;
    }

    CStatus init() override {
        DaoGraph::DAOGRAPH_ECHO("==> [%s] [init] get conn id = [%d]",
                            this->getName().c_str(), conn_id_);
        return CStatus();
    }

    CStatus run() override {
        std::shared_ptr<MyMessageParam> mp = nullptr;    // 通过智能指针类型，接收一个消息。相比value接收(Recv or Sub)，性能会好一些
        CStatus status = DAOGRAPH_SUB_MPARAM(MyMessageParam, conn_id_, mp)
        if (!status.isOK()) {
            DaoGraph::DAOGRAPH_ECHO("[%s] recv message error", this->getName().c_str());
            return status;
        }

        DaoGraph::DAOGRAPH_ECHO("[%s] conn id = [%d], num = [%d], info = [%s]",
                            this->getName().c_str(), conn_id_, mp->num, mp->info.c_str());
        return status;
    }

private:
    CIndex conn_id_;
};

#endif //DAOGRAPH_MYSUBMESSAGENODE_H
