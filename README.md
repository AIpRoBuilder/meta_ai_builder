中文 | [English Readme](README_en.md)

# DaoGraph 说明文档

## 一. 安装

### Python 安装

如果只需要 Python 绑定，可以直接从当前仓库安装。任选一种方式即可：

```bash
# pip
python3 -m pip install "git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"

# uv
uv add "pydaograph @ git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"

# poetry
poetry add "git+https://github.com/AIpRoBuilder/meta_ai_builder.git#subdirectory=python"
```

如果已经克隆仓库，也可以在仓库根目录执行 `python3 -m pip install ./python`。更完整的先决条件、可编辑安装和 JSON 示例见 [python/README.md](python/README.md)。

### C++ 安装

DaoGraph 的 C++ 版本采用源码集成方式，最直接的使用方式是先拉取仓库并完成本地构建：

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
cmake -S . -B build
cmake --build build -j
./build/tutorial/T00-HelloDaoGraph
```

如果要在自己的 CMake 工程中引入 DaoGraph，可直接复用 [cmake/DaoGraph-env-include.cmake](cmake/DaoGraph-env-include.cmake)。完整的构建与集成说明见 [COMPILE.md](COMPILE.md)。

### Docker 安装

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
docker build -f dockerfile -t daograph .
docker run --rm -it -v "$PWD":/home/DaoGraph daograph /bin/bash
```

如果是 Linux 环境，也可以直接运行仓库中的 [DaoGraph-docker-env.sh](DaoGraph-docker-env.sh)。

## 二. 示例

### C++ 示例

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

如上图所示，图结构执行的时候，首先执行 `a` 节点。`a` 节点执行完毕后，并行执行 `b` 和 `c` 节点。`b` 和 `c` 节点全部执行完毕后，再执行 `d` 节点。

### Python 示例

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

### Python JSON 流程示例

可以通过 JSON 描述节点和依赖关系来构建流水线。示例 JSON 文件位于 [python/tutorial/example.json](python/tutorial/example.json)，更完整的 Python 用法说明见 [python/README.md](python/README.md)。

## 三. 使用指引

1. 阅读 [COMPILE.md](COMPILE.md) 查看 C++ 构建、集成与环境准备。
2. 运行 `./DaoGraph-run-tutorials.sh`，按顺序查看 [tutorial](tutorial) 下的 `T00` 到 `T31` 教程示例。
3. 运行 `./DaoGraph-run-examples.sh`，查看 [example](example) 下的 `E01` 到 `E05` 示例。
4. 使用 Python 接口时，进入 [python](python) 目录，阅读 [python/README.md](python/README.md)，并运行 `./pydaograph-run-tutorials.sh`。
5. 需要查看核心接口时，可从 [src/DaoGraph.h](src/DaoGraph.h) 和 [src](src) 开始。

## 四. 许可证

本项目基于 Apache License 2.0 发布。详见 [LICENSE](LICENSE)。
