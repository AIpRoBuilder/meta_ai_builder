/***************************

@File: TestGAspects.h
@Time: 2023/12/29 21:59
@Desc: 
***************************/

#ifndef DAOGRAPH_TESTGASPECTS_H
#define DAOGRAPH_TESTGASPECTS_H

#include "TestGParams.h"
#include "TestCommonDefine.h"

class TestMaterialAdd1GAspect : public DaoGraph::GAspect {
public:
    CStatus beginRun() override {
        g_test_node_cnt++;
        return CStatus();
    }

    CVoid finishRun(const CStatus& curStatus) override {
        g_test_node_cnt++;
    }
};


class TestAdd1ByParamGAspect : public DaoGraph::GAspect {
public:
    CStatus beginRun() override {
        auto p = getGParamWithNoEmpty<TestAdd1GParam>(g_add1_by_param_key);
        {
            DAOGRAPH_PARAM_WRITE_CODE_BLOCK(p);
            p->cnt_++;
        }
        return CStatus();
    }
};

#endif //DAOGRAPH_TESTGASPECTS_H
