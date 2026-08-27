[中文](README.md) | English Readme

# DaoGraph README

## 1. Install

### Python

If you only need the Python bindings, install them directly from this repository with one of the following commands:

```bash
# pip
python3 -m pip install "git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"

# uv
uv add "pydaograph @ git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"

# poetry
poetry add "git+https://github.com/AIpRoBuilder/meta_ai_builder.git#subdirectory=python"
```

If you already cloned the repository, you can also run `python3 -m pip install ./python` from the repository root. See [python/README.md](python/README.md) for prerequisites, editable installs, and JSON workflow details.

### C++

The C++ library is consumed from source. The quickest way to build the tutorials and examples locally is:

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
cmake -S . -B build
cmake --build build -j
./build/tutorial/T00-HelloDaoGraph
```

To embed DaoGraph in your own CMake project, reuse [cmake/DaoGraph-env-include.cmake](cmake/DaoGraph-env-include.cmake). The fuller build and integration notes are in [COMPILE.md](COMPILE.md).

### Docker

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
docker build -f dockerfile -t daograph .
docker run --rm -it -v "$PWD":/home/DaoGraph daograph /bin/bash
```

On Linux, you can also use [DaoGraph-docker-env.sh](DaoGraph-docker-env.sh).

## 2. Examples

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

![DaoGraph Demo](doc/image/DaoGraph%20Demo.jpg)

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

You can build a pipeline from JSON definitions. A sample JSON file is available at [python/tutorial/example.json](python/tutorial/example.json), and the fuller Python workflow notes are in [python/README.md](python/README.md).

## 3. Guide

1. Read [COMPILE.md](COMPILE.md) for C++ build, integration, and environment setup.
2. Run `./DaoGraph-run-tutorials.sh` to walk through the C++ tutorials in [tutorial](tutorial).
3. Run `./DaoGraph-run-examples.sh` to inspect the larger examples in [example](example).
4. For Python usage, open [python/README.md](python/README.md) and run `./pydaograph-run-tutorials.sh` inside [python](python).
5. Start from [src/DaoGraph.h](src/DaoGraph.h) when you want the public C++ entry point.

## 4. License

This project is released under the Apache License 2.0. See [LICENSE](LICENSE).
