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

1. Load a JSON file with `buildFromJson` or a JSON string with `buildFromJsonStr`, as shown above.
2. A sample JSON file is available in [tutorial/example.json](tutorial/example.json).
3. The JSON root must contain a `nodes` array.
4. Each node requires `name` and `type`.
5. Optional fields include `meta_type`, `loop`, and `depends`.
6. `meta_type` defaults to `GNode` when omitted.
7. When `meta_type` is `GCondition`, `connect_elements` is required and should be an array of internal branch elements.

## License

`pydaograph` is released under the Apache License 2.0. See [../LICENSE](../LICENSE).
