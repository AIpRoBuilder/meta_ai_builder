[中文](README.md) | English Readme

# DaoGraph README

## 1. Examples

### C++ example

```cpp
#include "DaoGraph.h"

using namespace DaoGraph;

class MyNode1 : public GNode {
public:
    CStatus run() override {
        printf("[%s], sleep for 1 second ...\n", this->getName().c_str());
        DAOGRAPH_SLEEP_SECOND(1)
        return CStatus();
    }
};

class MyNode2 : public GNode {
public:
    CStatus run() override {
        printf("[%s], sleep for 2 second ...\n", this->getName().c_str());
        DAOGRAPH_SLEEP_SECOND(2)
        return CStatus();
    }
};


int main() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");
    pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");
    pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");

    pipeline->process();

    GPipelineFactory::remove(pipeline);

    return 0;
}
```

![DaoGraph Demo](https://github.com/Xiechuxi/DaoGraph/blob/main/doc/image/DaoGraph%20Demo.jpg)

As shown above, `a` runs first. Then `b` and `c` run in parallel. `d` runs after both `b` and `c` finish.

### Python example

```python
import time
from datetime import datetime

from pydaograph import GNode, GPipeline, CStatus


class MyNode1(GNode):
    def run(self):
        print("[{0}] {1}, enter MyNode1 run function. Sleep for 1 second ... ".format(datetime.now(), self.getName()))
        time.sleep(1)
        return CStatus()

class MyNode2(GNode):
    def run(self):
        print("[{0}] {1}, enter MyNode2 run function. Sleep for 2 second ... ".format(datetime.now(), self.getName()))
        time.sleep(2)
        return CStatus()


if __name__ == '__main__':
    pipeline = GPipeline()
    a, b, c, d = MyNode1(), MyNode2(), MyNode1(), MyNode2()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {a}, "nodeC")
    pipeline.registerGElement(d, {b, c}, "nodeD")

    pipeline.process()
```

### Python JSON workflow example

You can build a pipeline from JSON definitions. See [python/tutorial/json_examples/example1.json](python/tutorial/json_examples/example1.json) and [python/tutorial/T30-JSON.py](python/tutorial/T30-JSON.py).

```bash
cd python
python tutorial/T30-JSON.py
```

## 2. Guide

1. Read [COMPILE.md](COMPILE.md) for build, install, and environment setup.
2. Run `./DaoGraph-run-tutorials.sh` to walk through the C++ tutorials in [tutorial](tutorial).
3. Run `./DaoGraph-run-examples.sh` to inspect the larger examples in [example](example).
4. For Python usage, open [python/README.md](python/README.md) and run `./pydaograph-run-tutorials.sh` inside [python](python).
5. Start from [src/DaoGraph.h](src/DaoGraph.h) when you want the public C++ entry point.
