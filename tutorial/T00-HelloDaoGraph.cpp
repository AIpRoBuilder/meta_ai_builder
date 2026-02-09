/***************************

@File: T00-HelloDaoGraph.cpp
@Time: 2021/6/24 1:03 上午
@Desc: 本例主要演示，GPipeline中注册GNode，并执行的流程
***************************/

#include "MyGNode/HelloDaoGraphNode.h"

using namespace DaoGraph;

void tutorial_hello_daograph() {
    GPipelinePtr pipeline = GPipelineFactory::create();    // 创建一个pipeline，用于执行图框架信息
    GElementPtr hcg = nullptr;

    pipeline->registerGElement<HelloDaoGraphNode>(&hcg);    // 注册一个HelloDaoGraphNode节点，功能就是打印 "Hello, DaoGraph." 信息
    pipeline->process();    // 运行pipeline
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_hello_daograph();
    return 0;
}
