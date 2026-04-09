"""

@File: T31-InputHandler
@Time: 2026/4/2
@Desc: 演示 setInputHandler + setWaitForInput + pipeline disrupt/proceed 的组合用法
"""

import time
from pathlib import Path

from pydaograph import CStatus, GPipeline, StdFutureCStatus, register_class
from pydaograph import GNode


@register_class
class HandlerGateNode(GNode):
    def __init__(self):
        super().__init__()
        self._command = ""
        self.setWaitForInput(True)
        self.setInputPrompt("[T31] waiting input from stdin ... ")
        self.setInputHandler(self._input_handler)

    def _input_handler(self, user_input: str) -> CStatus:
        command = user_input.strip()
        self._command = command
        print(f"[{self.getName()}] setInputHandler receive: {command}")
        if command == "error":
            return CStatus(1001, "manual error from input handler")
        return CStatus()

    def run(self) -> CStatus:
        if self._command == "skip":
            print(f"[{self.getName()}] skip node work by handler command")
            return CStatus()

        print(f"[{self.getName()}] handler command = {self._command or '<empty>'}")
        time.sleep(0.3)
        return CStatus()

    def clone(self):
        return self


@register_class
class DownstreamNode(GNode):
    def run(self) -> CStatus:
        print(f"[{self.getName()}] downstream node running")
        time.sleep(0.3)
        return CStatus()

    def clone(self):
        return self


def tutorial_input_handler():
    pipeline = GPipeline()
    json_path = Path(__file__).with_name("T31-InputHandler.json")

    status = pipeline.buildFromJson(str(json_path))
    if status.isErr():
        print(f"buildFromJson failed: {status.getInfo()}")
        return

    status = pipeline.init()
    if status.isErr():
        print(f"pipeline init error: {status.getInfo()}")
        return

    pipeline.run()
    time.sleep(1.2)
    print("[main] PROCEED pipeline")
    pipeline.proceed()
    pipeline.destroy()


if __name__ == '__main__':
    tutorial_input_handler()
