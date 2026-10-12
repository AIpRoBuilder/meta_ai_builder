/***************************

@File: test-functional-06.cpp
@Time: 2026/4/8
@Desc: 验证 setWaitFunc 接口可配置，等待函数返回true后正常继续执行
***************************/

#include <atomic>

#include "../_Materials/TestInclude.h"

using namespace DaoGraph;

namespace {
const int WAIT_TRUE_AT_CALL = 3;
std::atomic<int> g_wait_calls { 0 };
CBool g_image_node_ran = false;
CBool g_ran_after_wait = false;
CBool g_downstream_ran = false;
}

class TestImageUploadNode : public GNode {
public:
    CStatus run() override {
        g_image_node_ran = true;
        g_ran_after_wait = g_wait_calls.load() >= WAIT_TRUE_AT_CALL;
        return CStatus();
    }
};

class TestImageDownstreamNode : public GNode {
public:
    CStatus run() override {
        g_downstream_ran = true;
        return CStatus();
    }
};

void test_functional_06() {
    g_wait_calls = 0;
    g_image_node_ran = false;
    g_ran_after_wait = false;
    g_downstream_ran = false;

    CStatus status;
    GPipelinePtr pipeline = GPipelineFactory::create();

    GElementPtr upload_node = nullptr;
    GElementPtr downstream_node = nullptr;

    status += pipeline->registerGElement<TestImageUploadNode>(&upload_node, {}, "upload_node");
    status += pipeline->registerGElement<TestImageDownstreamNode>(&downstream_node, {upload_node}, "downstream_node");
    if (status.isErr()) {
        DAOGRAPH_ECHO("test-functional-06 register failed, err = [%s]", status.getInfo().c_str());
        GPipelineFactory::remove(pipeline);
        return;
    }

    upload_node->setWaitFunc([] {
        return ++g_wait_calls >= WAIT_TRUE_AT_CALL;
    });

    status = pipeline->process();
    if (status.isErr()) {
        DAOGRAPH_ECHO("test-functional-06 process failed, err = [%s]", status.getInfo().c_str());
    }

    if (!g_image_node_ran || !g_ran_after_wait || !g_downstream_ran) {
        DAOGRAPH_ECHO("test-functional-06 failed, image node ran = [%d], ran after wait = [%d], downstream ran = [%d]",
                      g_image_node_ran,
                      g_ran_after_wait,
                      g_downstream_ran);
    }

    GPipelineFactory::remove(pipeline);
}

int main() {
    test_functional_06();
    return 0;
}
