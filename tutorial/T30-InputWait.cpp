/***************************

@File: T30-InputWait.cpp
@Time: 2026/4/2
@Desc: 本例子主要展示，GElement通过自定义等待函数，在函数返回true之后再执行，并驱动后继节点继续执行
***************************/

#include <atomic>
#include <thread>

#include "DaoGraph.h"

using namespace DaoGraph;

class WaitGateNode : public GNode {
public:
    CStatus run() override {
        DAOGRAPH_ECHO("[%s] wait func returned true, run node work ...", this->getName().c_str());
        DAOGRAPH_SLEEP_MILLISECOND(400)
        return CStatus();
    }
};


class DownstreamNode : public GNode {
public:
    CStatus run() override {
        DAOGRAPH_ECHO("[%s] downstream node running after WaitGateNode.", this->getName().c_str());
        return CStatus();
    }
};


void tutorial_input_wait() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr gate = nullptr;
    GElementPtr downstream = nullptr;

    CStatus status = pipeline->registerGElement<WaitGateNode>(&gate, {}, "wait_gate");
    status += pipeline->registerGElement<DownstreamNode>(&downstream, {gate}, "downstream");
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
        GPipelineFactory::remove(pipeline);
        return;
    }

    std::atomic<CBool> ready { false };
    gate->setWaitFunc([&ready] {
        return ready.load();    // 返回 true 之前，wait_gate 不会执行
    });

    // 模拟外部事件：1s 后，等待条件被满足
    std::thread notifier([&ready] {
        DAOGRAPH_SLEEP_MILLISECOND(1000)
        DAOGRAPH_ECHO("[notifier] condition is ready.");
        ready = true;
    });

    status = pipeline->process();
    notifier.join();
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_input_wait();
    return 0;
}
