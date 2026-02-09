/***************************

@File: MySendMessageNode.h
@Time: 2022/12/27 00:05
@Desc: 
***************************/

#ifndef DAOGRAPH_MYSENDMESSAGENODE_H
#define DAOGRAPH_MYSENDMESSAGENODE_H

#include "DaoGraph.h"
#include "../MyParams/MyMessageParam.h"

class MySendMessageNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        /**
         * 在 v3.1.2 版本，修改了message的使用方法
         * 仅通过 std::shared_ptr<T> 传递参数
         */
        std::shared_ptr<MyMessageParam> mp(new MyMessageParam());

        mp->num = (num_++) * 10;
        mp->info = "this is a test send info, num = " + std::to_string(mp->num);
        /**
         * 在v2.5.1版本，增加了 GMessagePushStrategy 策略，不兼容之前版本
         * 如果需要跟之前逻辑保持一致，直接设定 DaoGraph::GMessagePushStrategy::WAIT 即可
         * 其他的选项功能，参考 URingBufferPushStrategy(GMessagePushStrategy的别名) 的说明
         */
        CStatus status = DAOGRAPH_SEND_MPARAM(MyMessageParam, "send-recv", mp, DaoGraph::GMessagePushStrategy::WAIT)
        return status;
    }

private:
    int num_ = 0;
};


#endif //DAOGRAPH_MYSENDMESSAGENODE_H
