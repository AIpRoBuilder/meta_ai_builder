"""

@File: TCollectNodeChildren.py
@Time: 2026/5/20
@Desc: Demonstrates collectNodeChildren / collectNodeChildrenFromStr —
       parsing a graph JSON to discover each node's direct child nodes,
       then building and running the pipeline from the same JSON.

Graph structure (T06-Condition equivalent):

    writeNodeA  ──►  conditionB  ──►  readNodeC  ──►  conditionD
                     (branch: b0,b1,b2)               (branch: d0,d1,d2)

In the JSON schema used here:
  - A group node's "depends" = its upstream pipeline dependencies.
  - A node becomes a child of a group when it lists that group in its OWN "depends".
"""

import json
from pydaograph import GPipeline, GNode, GCondition, register_class


# ---------------------------------------------------------------------------
# Node definitions
# ---------------------------------------------------------------------------

@register_class
class WriteParamNode(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] writing param")
        return CStatus()

    def clone(self):
        return self


@register_class
class ReadParamNode(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] reading param")
        return CStatus()

    def clone(self):
        return self


@register_class
class BranchNodeB0(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] branch executing")
        return CStatus()

    def clone(self):
        return self


@register_class
class BranchNodeB1(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] branch executing")
        return CStatus()

    def clone(self):
        return self


@register_class
class BranchNodeB2(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] branch executing")
        return CStatus()

    def clone(self):
        return self


@register_class
class BranchNodeD0(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] branch executing")
        return CStatus()

    def clone(self):
        return self


@register_class
class BranchNodeD1(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] branch executing")
        return CStatus()

    def clone(self):
        return self


@register_class
class BranchNodeD2(GNode):
    def run(self):
        from pydaograph import CStatus
        print(f"[{self.getName()}] branch executing")
        return CStatus()

    def clone(self):
        return self


# ---------------------------------------------------------------------------
# Condition definitions (always picks index 0)
# ---------------------------------------------------------------------------

@register_class
class MyConditionB(GCondition):
    def choose(self):
        return 0

    def clone(self):
        return self


@register_class
class MyConditionD(GCondition):
    def choose(self):
        return 0

    def clone(self):
        return self


# ---------------------------------------------------------------------------
# Graph JSON
# ---------------------------------------------------------------------------

GRAPH_JSON = json.dumps({
    "nodes": [
        # ── source ──────────────────────────────────────────────────────────
        {"name": "WriteParamNode",  "type": "WriteParamNode",  "loop": 1},

        # ── condition group B (upstream: WriteParamNode) ─────────────────────
        {"name": "MyConditionB",    "type": "MyConditionB",    "meta_type": "GCondition",
         "depends": ["WriteParamNode"]},
        # branch children of MyConditionB
        {"name": "BranchNodeB0",    "type": "BranchNodeB0",    "depends": ["MyConditionB"]},
        {"name": "BranchNodeB1",    "type": "BranchNodeB1",    "depends": ["MyConditionB"]},
        {"name": "BranchNodeB2",    "type": "BranchNodeB2",    "depends": ["MyConditionB"]},

        # ── mid node ────────────────────────────────────────────────────────
        {"name": "ReadParamNode",   "type": "ReadParamNode",   "depends": ["BranchNodeB0","BranchNodeB1","BranchNodeB2"]}
    ]
})


# ---------------------------------------------------------------------------
# Demo
# ---------------------------------------------------------------------------

def demo_collect_node_children():
    # 1) Collect the parent -> children mapping using a throw-away pipeline
    collect_pipeline = GPipeline()
    status, children_map = collect_pipeline.collectNodeChildrenFromStr(GRAPH_JSON)
    if status.isErr():
        print(f"collectNodeChildrenFromStr failed: {status.getInfo()}")
        return

    print("=== Node -> Children map ===")
    for node_name, children in sorted(children_map.items()):
        child_str = ", ".join(children) if children else "<none>"
        print(f"  {node_name:15s} -> [{child_str}]")
    print()

    # 2) Build a fresh pipeline from the same JSON
    pipeline = GPipeline()
    status = pipeline.buildFromJsonStr(GRAPH_JSON)
    if status.isErr():
        print(f"buildFromJsonStr failed: {status.getInfo()}")
        return

    # 3) Init and run
    pipeline.init()
    for i in range(3):
        status = pipeline.run()
        print(f"=== run {i + 1}: status code = {status.getCode()} ===")
    pipeline.destroy()


if __name__ == "__main__":
    demo_collect_node_children()
