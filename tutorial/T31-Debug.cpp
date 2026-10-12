/***************************

@File: T31-Debug.cpp
@Time: 2026/4/12
@Desc: 本例子主要展示，通过 setWaitFunc 给 element 设置断点，在 pipeline 运行期间逐个节点单步调试
***************************/

#include <atomic>
#include <map>
#include <string>

#include "MyGNode/MyNode1.h"

using namespace DaoGraph;

/** 一个简单的调试器：element 命中断点后会停住，直到调试器放行 */
class Debugger {
public:
    /** 返回给 element 的等待函数，命中断点后一直返回 false，直到被 release */
    GElement::GWaitFunc breakpoint(const std::string& name) {
        Breakpoint* bp = &breakpoints_[name];
        return [bp, name] {
            if (!bp->hit_.exchange(true)) {
                DAOGRAPH_ECHO("[debug] breakpoint hit at [%s], paused.", name.c_str());
            }
            return bp->released_.load();
        };
    }

    void waitHit(const std::string& name) {
        Breakpoint& bp = breakpoints_.at(name);
        while (!bp.hit_.load()) {
            DAOGRAPH_SLEEP_MILLISECOND(10)
        }
    }

    void release(const std::string& name) {
        breakpoints_.at(name).released_ = true;
    }

private:
    struct Breakpoint {
        std::atomic<CBool> hit_ { false };
        std::atomic<CBool> released_ { false };
    };

    std::map<std::string, Breakpoint> breakpoints_;
};


void tutorial_debug() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a = nullptr;
    GElementPtr b = nullptr;
    GElementPtr c = nullptr;

    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    status += pipeline->registerGElement<MyNode1>(&b, {a}, "nodeB");
    status += pipeline->registerGElement<MyNode1>(&c, {b}, "nodeC");
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
        GPipelineFactory::remove(pipeline);
        return;
    }

    // 在 nodeB 和 nodeC 前设置断点，nodeA 不受影响
    Debugger debugger;
    b->setWaitFunc(debugger.breakpoint("nodeB"));
    c->setWaitFunc(debugger.breakpoint("nodeC"));

    status = pipeline->init();
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
        GPipelineFactory::remove(pipeline);
        return;
    }

    auto result = pipeline->asyncRun();
    const char* breakpoints[] = { "nodeB", "nodeC" };
    for (const char* name : breakpoints) {
        debugger.waitHit(name);
        DAOGRAPH_ECHO("[debug] inspecting pipeline while [%s] is paused ...", name);
        DAOGRAPH_SLEEP_MILLISECOND(800)
        DAOGRAPH_ECHO("[debug] step over [%s].", name);
        debugger.release(name);
    }

    status = result.get();
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
    }

    pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_debug();
    return 0;
}
