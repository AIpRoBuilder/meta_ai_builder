"""

@File: T30-InputWaitAndDisrupt
@Time: 2026/4/2
@Desc: 演示 GElement 等待用户输入执行，以及 pipeline disrupt/proceed 控制
"""

import time

from pydaograph import CStatus, GPipeline, StdFutureCStatus

from MyGNode.MyNode1 import MyNode1


class InputGateNode(MyNode1):
    def __init__(self):
        super().__init__()
        self._command = ""

    def onInput(self, user_input: str) -> CStatus:
        self._command = user_input.strip()
        print(f"[{self.getName()}] receive input: {self._command}")
        return CStatus()

    def run(self) -> CStatus:
        if self._command == "skip":
            print(f"[{self.getName()}] skip node work by user command")
            return CStatus()

        print(f"[{self.getName()}] run with command: {self._command or '<empty>'}")
        time.sleep(0.4)
        return CStatus()


class FastNode(MyNode1):
    def run(self) -> CStatus:
        print(f"[{self.getName()}] running ...")
        time.sleep(1.0)
        return CStatus()


class HandlerGateNode(MyNode1):
    def __init__(self):
        super().__init__()
        self._command = ""

    def run(self) -> CStatus:
        if self._command == "skip":
            print(f"[{self.getName()}] skip node work by handler command")
            return CStatus()

        print(f"[{self.getName()}] handler command = {self._command or '<empty>'}")
        time.sleep(0.3)
        return CStatus()


def tutorial_input_wait():
    pipeline = GPipeline()
    gate, node_b = InputGateNode(), MyNode1()

    pipeline.registerGElement(gate, set(), "input_gate")
    pipeline.registerGElement(node_b, {gate}, "node_b")

    gate.setWaitForInput(True)
    gate.setInputPrompt("[T30] enter command for input_gate (type skip to bypass): ")

    pipeline.process()


def tutorial_input_wait_with_handler():
    pipeline = GPipeline()
    gate, node_b = HandlerGateNode(), MyNode1()

    pipeline.registerGElement(gate, set(), "handler_gate")
    pipeline.registerGElement(node_b, {gate}, "node_b")

    def input_handler(user_input: str) -> CStatus:
        command = user_input.strip()
        gate._command = command
        print(f"[{gate.getName()}] setInputHandler receive: {command}")
        if command == "error":
            return CStatus(1001, "manual error from input handler")
        return CStatus()

    gate.setWaitForInput(True)
    gate.setInputPrompt("[T30-handler] enter command (skip|error|any): ")
    gate.setInputHandler(input_handler)

    status = pipeline.process()
    if status.isErr():
        print(f"pipeline status error: {status.getInfo()}")



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
    print("\n===== tutorial_input_wait_with_handler =====")
    tutorial_input_wait_with_handler()
    # print("\n===== tutorial_disrupt_proceed =====")
    # tutorial_disrupt_proceed()
