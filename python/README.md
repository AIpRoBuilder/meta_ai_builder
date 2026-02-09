JSON Workflow Building (Python)
--------------------------------

You can build a DaoGraph pipeline from JSON (or Python dict) using the new `buildFromJson` method on `GPipeline`.

Example:

```
from pydaograph import GPipeline, GFunction

def run_a(param):
    print('run A')

def run_b(param):
    print('run B')

pipeline = GPipeline()
spec = {
    'elements': [
        {'name': 'nodeA', 'type': 'GFunction', 'func': run_a},
        {'name': 'nodeB', 'type': 'GFunction', 'depends': ['nodeA'], 'func': run_b}
    ]
}
pipeline.buildFromJson(spec)
pipeline.process()
```

Notes:
- `buildFromJson` accepts JSON strings or Python dictionaries.
- `type` may be a class name exposed in the `pydaograph` module (e.g. `GFunction`) or a dotted Python path to a class (e.g. `tutorial.MyGNode.HelloDaoGraphNode.HelloDaoGraphNode`).
- `func` in `GFunction` can be a Python callable or module path string referencing a callable.
C++ JSON support:
- To use `GPipeline::loadFromJsonString` (C++ side), enable `DAOGRAPH_USE_NLOHMANN_JSON` in CMake and ensure `nlohmann_json` is available on the system. The C++ loader parses JSON strings and tries to construct C++ pipeline elements. This is best when `clz` values are `typeid(T).name()` for registered types.

