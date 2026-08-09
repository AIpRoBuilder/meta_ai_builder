中文 | [English Readme](README_en.md)

# DaoGraph 说明文档

## 一. 示例

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

![DaoGraph Demo](https://github.com/Xiechuxi/DaoGraph/blob/main/doc/image/DaoGraph%20Demo.jpg)

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

可以通过 JSON 描述节点和依赖关系来构建流水线。示例文件位于 [python/tutorial/json_examples/example1.json](python/tutorial/json_examples/example1.json) 和 [python/tutorial/T30-JSON.py](python/tutorial/T30-JSON.py)。

```bash
cd python
python tutorial/T30-JSON.py
```

## 二. 使用指引

1. 阅读 [COMPILE.md](COMPILE.md) 完成编译、安装与环境准备。
2. 运行 `./DaoGraph-run-tutorials.sh`，按顺序查看 [tutorial](tutorial) 下的 `T00` 到 `T30` 教程示例。
3. 运行 `./DaoGraph-run-examples.sh`，查看 [example](example) 下的 `E01` 到 `E05` 示例。
4. 使用 Python 接口时，进入 [python](python) 目录，阅读 [python/README.md](python/README.md)，并运行 `./pydaograph-run-tutorials.sh`。
5. 需要查看核心接口时，可从 [src/DaoGraph.h](src/DaoGraph.h) 和 [src](src) 开始。
