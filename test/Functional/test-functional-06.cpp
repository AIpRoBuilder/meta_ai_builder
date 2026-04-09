/***************************

@File: test-functional-06.cpp
@Time: 2026/4/8
@Desc: 验证 setWaitForInput 接口可配置并正常继续执行
***************************/

#include "../_Materials/TestInclude.h"

using namespace DaoGraph;

namespace {
CBool g_image_node_ran = false;
CBool g_downstream_ran = false;
}

class TestImageUploadNode : public GNode {
public:
    CStatus run() override {
        g_image_node_ran = true;
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
    g_image_node_ran = false;
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

    upload_node->setWaitForInput(false);

    status = pipeline->process();
    if (status.isErr()) {
        DAOGRAPH_ECHO("test-functional-06 process failed, err = [%s]", status.getInfo().c_str());
    }

    if (!g_image_node_ran || !g_downstream_ran) {
        DAOGRAPH_ECHO("test-functional-06 failed, image node ran = [%d], downstream ran = [%d]",
                      g_image_node_ran,
                      g_downstream_ran);
    }

    GPipelineFactory::remove(pipeline);
}

int main() {
    test_functional_06();
    return 0;
}
