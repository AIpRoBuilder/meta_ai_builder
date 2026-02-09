/***************************

@File: TestGNodes.h
@Time: 2023/12/3 17:40
@Desc: 
***************************/

#ifndef DAOGRAPH_TESTGNODES_H
#define DAOGRAPH_TESTGNODES_H

#include <memory>

#include "TestCommonDefine.h"
#include "TestGParams.h"

class TestAdd1GNode : public DaoGraph::GNode {
public:
    CStatus init() override {
        g_test_node_cnt = 0;
        return CStatus();
    }

    CStatus run() override {
        g_test_node_cnt++;
        return CStatus();
    }
};


class TestReturnErrorGNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        return CStatus("test error return, no real problem");
    }
};

class TestAdd1ByParamGNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        auto p = getGParamWithNoEmpty<TestAdd1GParam>(g_add1_by_param_key);
        {
            DAOGRAPH_PARAM_WRITE_CODE_BLOCK(p)
            p->cnt_++;
        }
        return CStatus();
    }
};


class TestRecvMessageGNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        std::shared_ptr<TestGMessageParam> mp = nullptr;
        CStatus status = DAOGRAPH_RECV_MPARAM_WITH_TIMEOUT(TestGMessageParam, g_test_message_key, mp, 200);
        if (status.isErr()) {
            DaoGraph::DAOGRAPH_ECHO("error message is [%s]",  status.getInfo().c_str());
            return status;
        }

        mp->num_++;
        DAOGRAPH_SEND_MPARAM(TestGMessageParam, g_test_message_key, mp, DaoGraph::GMessagePushStrategy::WAIT)
        return status;
    }
};

#endif //DAOGRAPH_TESTGNODES_H
