# PyDaoGraph

## Install

`pydaograph` is the Python wrapper for DaoGraph and is built from the sources in this repository.

### Prerequisites

- Python 3.8 or newer
- `pip`, `uv`, or `poetry`
- A C++ build toolchain
- The matching CPython development headers for the interpreter that will build the extension

Common toolchains:

- macOS: Xcode Command Line Tools
- Linux: `gcc`, `g++`, and the Python development package for your interpreter such as `python3-devel` or `python3-dev`
- Windows: Visual Studio Build Tools

On Linux, the Python headers must match the interpreter version that is building the extension. For example, if you install with Python 3.11, make sure `Python.h` is provided by the corresponding system package before running `pip install`.

### Install With pip

Install directly from this repository:

```bash
python3 -m pip install "git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"
```

Install from a local clone:

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
python3 -m pip install ./python
```

For development, install the package in editable mode instead:

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
python3 -m pip install -e ./python
```

### Install With uv

Add `pydaograph` to an existing `uv` project:

```bash
uv add "pydaograph @ git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"
```

Install from a local clone into the active environment:

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
uv pip install ./python
```

### Install With poetry

Add `pydaograph` as a Poetry dependency:

```bash
poetry add "git+https://github.com/AIpRoBuilder/meta_ai_builder.git#subdirectory=python"
```

Install from a local clone:

```bash
git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
cd meta_ai_builder
poetry add ./python
```

### Verify The Installation

```bash
python -c "from pydaograph import GPipeline; print('pydaograph installed')"
```

## JSON Workflow

### Example

`GPipeline` supports two JSON-loading APIs:

- `buildFromJsonStr(jsonStr, nodeClasses)`: parse a JSON string and construct nodes
  from a dictionary mapping **JSON node names** to Python classes.
- `buildFromJson(jsonPath, nodeClasses)`: load a JSON file with the same dictionary.

Classes are called with no arguments. Each call must create a fresh `GElement`
(normally a `GNode` or `GCondition` subclass). Use a zero-argument factory such as
`lambda: MyNode(config)` when constructor arguments are needed. Neither global
registration nor `clone()` is required. The pipeline retains the Python node
instances for its lifetime, including their overrides and instance attributes.

```python
import json
from pydaograph import CStatus, GNode, GPipeline

class MyNode(GNode):
    def run(self):
        print(self.getName())
        return CStatus()

graph_json_str = json.dumps({
    "nodes": [
        {"name": "A"},
        {"name": "B", "depends": ["A"], "loop": 2}
    ]
})
node_classes = {"A": MyNode, "B": MyNode}

pipeline = GPipeline()
status = pipeline.buildFromJsonStr(graph_json_str, node_classes)
if status.isErr():
    raise RuntimeError(status.getInfo())

status = pipeline.process()  # init, run, destroy
if status.isErr():
    raise RuntimeError(status.getInfo())
```

See [tutorial/TBuildFromJsonWithNodeClasses.py](tutorial/TBuildFromJsonWithNodeClasses.py)
and its [JSON graph](tutorial/TBuildFromJsonWithNodeClasses.json) for a runnable
example of both string and file loading.

For asynchronous full-lifecycle execution, use `pipeline.asyncProcess()` and
check the `CStatus` returned by the future's `get()` method. This initializes,
runs, and destroys the pipeline, just like `process()`; do not call `destroy()`
again. `asyncRun()` only runs an already initialized pipeline: call `init()`
first, retrieve the result with `future.get()`, then call `destroy()` when no
more runs are needed. Keep the pipeline alive until the future completes.

The one-argument forms remain supported for existing code: `buildFromJson(jsonPath)`
and `buildFromJsonStr(jsonStr)` resolve each node's `type` through the global
`GElement` registry. Supplying a dictionary uses only that dictionary; it does
not modify or fall back to the registry, and JSON `type` values are ignored.

### Guide

1. Load a JSON file with `buildFromJson` or a JSON string with `buildFromJsonStr`, as shown above.
2. Supply a class or factory for every node name, including condition groups and branches.
3. The JSON root must contain a `nodes` array.
4. Each node requires a unique, nonempty `name`. `type` is required only for the legacy one-argument APIs.
5. Optional fields include `meta_type`, `loop`, and `depends`.
6. `meta_type` defaults to `GNode` when omitted.
7. When `meta_type` is `GCondition`, map its name to a `GCondition` subclass. Its
   internal branches are other nodes whose `depends` arrays list the group's
   name, in JSON declaration order. Downstream dependencies on a branch are
   promoted to its owning condition group; branches are not run independently.

The C++ overloads take `GPipeline::GNodeClassMap`, an
`std::unordered_map<std::string, GElement::GElementCreator>`. Map node names to
constructors such as `[]() -> GElement* { return new MyNode(); }`. The pipeline
owns the resulting fresh elements, including those created before a build error.

### Concurrent Requests From One JSON Graph

See [tutorial/TConcurrentJsonRequests.py](tutorial/TConcurrentJsonRequests.py)
and its single [JSON graph](tutorial/TConcurrentJsonRequests.json).
Run the Python tutorial from your checkout; four request workers share a pool of
two pipelines managed by `GPipelineManager`.

`RequestPipelinePool(size=2)` builds two independent instances from the same JSON,
adds each to the manager once, and calls `manager.init()` once. Each pipeline has
its own nodes and `RequestParam` registered under the same key, `"request"`.

Each call to `pool.handle_request(request_id, values)`:

1. Calls `manager.fetch()` to lease a pipeline exclusively; waits when none is free.
2. Copies the request inputs into that pipeline's parameter and calls `pipeline.run()`.
3. Captures an immutable, detached `Response` before returning the slot.
4. Clears **all** request inputs, scratch data, and response references, then calls
    `manager.release(pipeline)` in `finally`, even when execution fails.

Do not call `manager.run()` after `fetch()`: it fetches another pipeline internally
and provides no opportunity to attach the current request's inputs or read its result.
The manager controls the free/used lists; a Python `Condition` supplies waiting
and coordinated shutdown. `close()` rejects new requests, waits for active leases,
then calls `manager.destroy()` and `manager.clear()`. Use the pool as a context
manager outside the request executor so workers finish before shutdown.

Nodes/parameters are reused sequentially within a slot, never concurrently across
requests. The JSON specifies `double_values` before `build_response`. The pool
calls `makeSerial()` to require a synchronous chain and run it on the requesting
thread without detached work. Request-level concurrency still comes from
`ThreadPoolExecutor`; Python threads do not guarantee CPU parallelism for pure
Python work.

The factory calls `creator()->clone()`: this tutorial registers empty factory-only
prototypes whose `clone()` creates a fresh execution node. The prototypes never
join a pipeline or receive request data. The pool retains its Python node wrappers
through `destroy()`, then releases them before the C++ nodes are freed.
This avoids the current binding's retained factory-wrapper/stale-identity issue
when repeatedly creating and deleting JSON pipelines. Do not use `clone(): return self`
for executing nodes in this request-serving pattern.

`run()` calls parameter `reset()` before returning. The tutorial clears scratch
data there while retaining the immutable response until the handler reads it;
the handler then calls `clear()` before releasing the pipeline. The nodes keep no
request data in instance fields. Adding node caches, timeouts, parallel branches,
or background events/daemons requires additional reset/draining rules before reuse.

The [regression tests](test/test_json_concurrent_requests.py) force overlapping
requests with a barrier, check fixed pool reuse and exact results, and exercise
failure recovery, excess-request waiting, and coordinated shutdown.

## License

`pydaograph` is released under the Apache License 2.0. See [../LICENSE](../LICENSE).
