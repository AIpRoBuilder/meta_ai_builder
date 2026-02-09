#include "DaoGraph.h"

using namespace DaoGraph;

class MyNode : public GNode {
public:
    static const char* staticClassName() { return "MyNode"; } // optional
    CStatus run() override { 
        DAOGRAPH_ECHO("Running MyNode");
        return CStatus(); 
    }
    std::string getClassName() const override { return staticClassName(); }
    GElement* clone() const override { return new MyNode(); }
};

// auto-register at load
DAOGRAPH_AUTO_REGISTER_GELEMENT(MyNode);

class OtherNode : public GNode {
public:
    static const char* staticClassName() { return "OtherNode"; } // optional
    CStatus run() override { 
        DAOGRAPH_ECHO("Running OtherNode");
        return CStatus();
    }
    std::string getClassName() const override { return staticClassName(); }
    GElement* clone() const override { return new OtherNode(); }
};

// auto-register at load
DAOGRAPH_AUTO_REGISTER_GELEMENT(OtherNode);

int main() {
    auto pipeline = GPipelineFactory::create();
    CStatus status;
    status += pipeline->buildFromJson("/Users/xiechuxi/Desktop/codes/meta_ai_builder/example/dag.json");
    status += pipeline->process();
    if (status.isErr()) {
        DAOGRAPH_ECHO("Pipeline process failed: %s", status.getInfo().c_str());
        GPipelineFactory::remove(pipeline);
        return -1;
    }
    DAOGRAPH_ECHO("Pipeline process succeeded: %s", status.getInfo().c_str());

    GPipelineFactory::remove(pipeline);
    return 0;
}