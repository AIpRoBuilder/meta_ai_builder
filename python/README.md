# PyDaoGraph JSON Workflow

## Example

`GPipeline` supports two JSON-loading APIs:

- `buildFromJson(jsonPath)`: load and parse from a JSON file path.
- `buildFromJsonStr(jsonStr)`: parse directly from a JSON string.

```python
from pydaograph import GPipeline

pipeline = GPipeline()

graph_json_str = '''
{
    "nodes": [
        {"name": "A", "type": "MyNode", "meta_type": "GNode", "loop": 1},
        {
            "name": "conditionB",
            "type": "MyCondition",
            "meta_type": "GCondition",
            "depends": ["A"],
            "loop": 1,
            "connect_elements": [
                {"name": "conditionNodeB0", "type": "MyNode1", "loop": 1},
                {"name": "conditionNodeB1", "type": "MyNode2", "loop": 1},
                {"name": "conditionNodeB2", "type": "MyNode1", "loop": 1}
            ]
        },
        {"name": "C", "type": "MyNode", "meta_type": "GNode", "depends": ["conditionB"], "loop": 1}
    ]
}
'''

pipeline.buildFromJsonStr(graph_json_str)

# Or parse from a JSON file path.
# pipeline.buildFromJson("/path/to/graph.json")

pipeline.process()
```

## Guide

1. Run the example from the `python` directory:

```bash
python tutorial/T30-JSON.py
```

2. Example files are available in [tutorial/json_examples/example1.json](tutorial/json_examples/example1.json) and [tutorial/T30-JSON.py](tutorial/T30-JSON.py).
3. The JSON root must contain a `nodes` array.
4. Each node requires `name` and `type`.
5. Optional fields include `meta_type`, `loop`, and `depends`.
6. `meta_type` defaults to `GNode` when omitted.
7. When `meta_type` is `GCondition`, `connect_elements` is required and should be an array of internal branch elements.
