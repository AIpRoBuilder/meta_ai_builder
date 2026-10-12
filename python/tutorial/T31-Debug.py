"""

@File: T31-Debug
@Time: 2026/4/12
@Desc: 演示通过 setWaitFunc 给 element 设置断点，在 pipeline 运行期间逐个节点单步调试
"""

import threading
import time

from pydaograph import GPipeline

from MyGNode.MyNode1 import MyNode1


class Debugger:
    """element 命中断点后会停住，直到调试器放行"""

    def __init__(self):
        self._hit = {}
        self._released = {}

    def breakpoint(self, name: str):
        """返回给 element 的等待函数，命中断点后一直返回 False，直到被 step_over"""
        hit, released = threading.Event(), threading.Event()
        self._hit[name], self._released[name] = hit, released

        def wait_func() -> bool:
            if not hit.is_set():
                hit.set()
                print(f"[debug] breakpoint hit at [{name}], paused.")
            return released.is_set()

        return wait_func

    def wait_hit(self, name: str):
        self._hit[name].wait()

    def step_over(self, name: str):
        self._released[name].set()


def tutorial_debug():
    pipeline = GPipeline()
    a, b, c = MyNode1(), MyNode1(), MyNode1()

    pipeline.registerGElement(a, set(), "nodeA")
    pipeline.registerGElement(b, {a}, "nodeB")
    pipeline.registerGElement(c, {b}, "nodeC")

    # 在 nodeB 和 nodeC 前设置断点，nodeA 不受影响
    debugger = Debugger()
    b.setWaitFunc(debugger.breakpoint("nodeB"))
    c.setWaitFunc(debugger.breakpoint("nodeC"))

    pipeline.init()
    future = pipeline.asyncRun()
    for name in ("nodeB", "nodeC"):
        debugger.wait_hit(name)
        print(f"[debug] inspecting pipeline while [{name}] is paused ...")
        time.sleep(0.8)
        print(f"[debug] step over [{name}].")
        debugger.step_over(name)

    future.wait()
    pipeline.destroy()


if __name__ == '__main__':
    tutorial_debug()
