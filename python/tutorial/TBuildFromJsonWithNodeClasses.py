"""Build independent pipelines from JSON and a node-name -> Python class map.

No global registration, decorators, prototypes or clone() methods are needed.
"""

from pathlib import Path

from pydaograph import CStatus, GNode, GPipeline


GRAPH_PATH = Path(__file__).with_suffix(".json")


class PrintNode(GNode):
    def run(self) -> CStatus:
        print(f"Running {self.getName()}")
        return CStatus()


class SummaryNode(GNode):
    def run(self) -> CStatus:
        print(f"{self.getName()}: all upstream nodes completed")
        return CStatus()


NODE_CLASSES = {
    "start": PrintNode,
    "repeat": PrintNode,
    "finish": SummaryNode,
}


def check_status(status: CStatus) -> None:
    if status.isErr():
        raise RuntimeError(status.getInfo())


def main() -> None:
    # JSON contains names/dependencies/loops; Python supplies implementation classes.
    graph_json = GRAPH_PATH.read_text(encoding="utf-8")
    pipeline = GPipeline()
    check_status(pipeline.buildFromJsonStr(graph_json, NODE_CLASSES))
    # asyncProcess() handles init/run/destroy; get() waits and returns CStatus.
    # asyncRun() only runs an already initialized pipeline.
    check_status(pipeline.asyncProcess().get())  # Do not destroy again.

    # File loading accepts the same map and creates fresh instances for each node.
    file_pipeline = GPipeline()
    check_status(file_pipeline.buildFromJson(str(GRAPH_PATH), NODE_CLASSES))
    check_status(file_pipeline.asyncProcess().get())


if __name__ == "__main__":
    main()