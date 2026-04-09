/***************************

@File: T30-InputWait.cpp
@Time: 2026/4/2
@Desc: 本例子主要展示，GElement等待用户输入后再执行，并驱动后继节点继续执行
***************************/

#include "DaoGraph.h"

using namespace DaoGraph;

class InputGateNode : public GNode {
public:
    CStatus onInput(const std::string& input) override {
        command_ = input;
        if ("skip" == command_) {
            skip_work_ = true;
        }
        return CStatus();
    }

    CStatus run() override {
        if (skip_work_) {
            DAOGRAPH_ECHO("[%s] receive [skip], bypass node work.", this->getName().c_str());
            return CStatus();
        }

        DAOGRAPH_ECHO("[%s] user command = [%s], run node work ...",
                      this->getName().c_str(),
                      command_.empty() ? "<empty>" : command_.c_str());
        DAOGRAPH_SLEEP_MILLISECOND(400)
        return CStatus();
    }

private:
    std::string command_;
    CBool skip_work_ { false };
};


class DownstreamNode : public GNode {
public:
    CStatus run() override {
        DAOGRAPH_ECHO("[%s] downstream node running after InputGateNode.", this->getName().c_str());
        return CStatus();
    }
};


void tutorial_input_wait() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr input = nullptr;
    GElementPtr downstream = nullptr;

    CStatus status = pipeline->registerGElement<InputGateNode>(&input, {}, "input_gate");
    status += pipeline->registerGElement<DownstreamNode>(&downstream, {input}, "downstream");
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
        GPipelineFactory::remove(pipeline);
        return;
    }

    input->setWaitForInput(true)
         ->setInputPrompt("[tutorial T30] enter command for input_gate (type skip to bypass): ");

    status = pipeline->process();
    if (!status.isOK()) {
        std::cout << status.getInfo() << std::endl;
    }

    GPipelineFactory::remove(pipeline);
}


int main() {
    tutorial_input_wait();
    return 0;
}
