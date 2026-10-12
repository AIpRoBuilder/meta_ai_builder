"""

@File: T30-InputWaitAndDisrupt
@Time: 2026/4/2
@Desc: 演示 GElement 通过自定义等待函数延迟执行，以及 pipeline disrupt/proceed 控制
"""

import threading
import time

from pydaograph import CStatus, GPipeline, StdFutureCStatus

from MyGNode.MyNode1 import MyNode1


class FastNode(MyNode1):
    def run(self) -> CStatus:
        print(f"[{self.getName()}] running ...")
        time.sleep(1.0)
        return CStatus()


def tutorial_input_wait():
    pipeline = GPipeline()
    gate, node_b = MyNode1(), MyNode1()

    pipeline.registerGElement(gate, set(), "wait_gate")
    pipeline.registerGElement(node_b, {gate}, "node_b")

    ready = threading.Event()
    gate.setWaitFunc(ready.is_set)    # wait_gate 不会执行，直到该函数返回 True

    # 模拟外部事件：1s 后，等待条件被满足
    threading.Timer(1.0, ready.set).start()

    pipeline.process()



def tutorial_disrupt_proceed():
    pipeline = GPipeline()
    a, b, c = FastNode(), FastNode(), FastNode()

    pipeline.registerGElement(a, set(), "node_a", 2)
    pipeline.registerGElement(b, {a}, "node_b", 2)
    pipeline.registerGElement(c, {b}, "node_c", 1)

    pipeline.init()
    future: StdFutureCStatus = pipeline.asyncRun()

    time.sleep(1.3)
    print("pipeline async run, DISRUPT.")
    pipeline.disrupt()

    time.sleep(1.8)
    print("pipeline async run, PROCEED.")
    pipeline.proceed()

    future.wait()
    pipeline.destroy()


if __name__ == '__main__':
    print("===== tutorial_input_wait =====")
    tutorial_input_wait()
    # print("\n===== tutorial_disrupt_proceed =====")
    # tutorial_disrupt_proceed()
