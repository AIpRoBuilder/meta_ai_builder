JSON Workflow Building (Python)
--------------------------------

`GPipeline` supports two JSON-loading APIs:

- `buildFromJson(jsonPath)`: load and parse from a JSON file path.
- `buildFromJsonStr(jsonStr)`: parse directly from a JSON string.

Example:

```python
from pydaograph import GPipeline

pipeline = GPipeline()

graph_json_str = '''
{
    "nodes": [
        {"name": "A", "type": "MyNode", "loop": 1},
        {"name": "B", "type": "MyNode", "depends": ["A"], "loop": 1}
    ]
}
'''

# Parse from JSON string
pipeline.buildFromJsonStr(graph_json_str)

# Or parse from JSON file path
# pipeline.buildFromJson("/path/to/graph.json")

pipeline.process()
```

Notes:
- JSON root must contain a `nodes` array.
- Each node requires `name` and `type`.
- Optional fields include `loop` and `depends`.

