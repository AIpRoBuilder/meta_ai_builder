"""JSON loaders accept per-build node classes without registration or cloning."""

from concurrent.futures import ThreadPoolExecutor
import gc
import json
from pathlib import Path
import subprocess
import sys
from tempfile import TemporaryDirectory
import textwrap
import unittest
import weakref

from pydaograph import CStatus, GCondition, GElement, GNode, GPipeline
from tutorial.TBuildFromJsonWithNodeClasses import GRAPH_PATH, NODE_CLASSES


class JsonNodeClassesTest(unittest.TestCase):
    def assert_ok(self, status):
        self.assertTrue(status.isOK(), status.getInfo())

    def make_node_class(self, events, references=None):
        class RecordingNode(GNode):
            def __init__(self):
                super().__init__()
                self.runs = 0
                if references is not None:
                    references.append(weakref.ref(self))

            def init(self):
                events.append(("init", self.getName()))
                return CStatus()

            def run(self):
                self.runs += 1
                events.append(("run", self.getName(), self.runs))
                return CStatus()

            def destroy(self):
                events.append(("destroy", self.getName()))
                return CStatus()

        return RecordingNode

    def test_string_loader_uses_names_dependencies_loops_and_keeps_nodes_alive(self):
        events, references = [], []
        node_class = self.make_node_class(events, references)
        # Reverse JSON order to verify that dependencies, not declaration order,
        # determine execution. "type" is optional and ignored when a map is supplied.
        graph = {"nodes": [
            {"name": "last", "depends": ["first"], "loop": 2, "type": "unregistered"},
            {"name": "first"},
        ]}
        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJsonStr(
            jsonStr=json.dumps(graph), nodeClasses={"first": node_class, "last": node_class}
        ))
        gc.collect()
        self.assertTrue(all(ref() is not None for ref in references))
        self.assertIsNot(references[0](), references[1]())
        self.assert_ok(pipeline.makeSerial())
        self.assert_ok(pipeline.process())
        self.assertEqual([event for event in events if event[0] == "run"], [
            ("run", "first", 1), ("run", "last", 1), ("run", "last", 2),
        ])
        self.assertEqual(sum(event[0] == "init" for event in events), 2)
        self.assertEqual(sum(event[0] == "destroy" for event in events), 2)
        del pipeline
        gc.collect()
        self.assertTrue(all(ref() is None for ref in references))

    def test_file_loader_accepts_class_map(self):
        events = []
        node_class = self.make_node_class(events)
        with TemporaryDirectory() as directory:
            path = Path(directory) / "graph.json"
            path.write_text(json.dumps({"nodes": [{"name": "file_node"}]}), encoding="utf-8")
            pipeline = GPipeline()
            self.assert_ok(pipeline.buildFromJson(str(path), {"file_node": node_class}))
            self.assert_ok(pipeline.process())
        self.assertIn(("run", "file_node", 1), events)

    def test_zero_argument_factory_can_supply_constructor_arguments(self):
        events = []

        class ConfiguredNode(GNode):
            def __init__(self, value):
                super().__init__()
                self.value = value

            def run(self):
                events.append((self.getName(), self.value))
                return CStatus()

        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJsonStr(
            '{"nodes": [{"name": "configured"}]}',
            {"configured": lambda: ConfiguredNode(42)},
        ))
        self.assert_ok(pipeline.process())
        self.assertEqual(events, [("configured", 42)])

    def test_condition_groups_and_branches_use_the_same_map(self):
        events, references = [], []
        node_class = self.make_node_class(events, references)

        class SelectSecond(GCondition):
            def choose(self):
                return 1

        graph = {"nodes": [
            {"name": "start"},
            {"name": "select", "meta_type": "GCondition", "depends": ["start"]},
            {"name": "left", "depends": ["select"]},
            {"name": "right", "depends": ["select"]},
            {"name": "finish", "depends": ["left", "right"]},
        ]}
        classes = {name: node_class for name in ("start", "left", "right", "finish")}
        classes["select"] = SelectSecond
        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJsonStr(json.dumps(graph), classes))
        self.assert_ok(pipeline.process())
        self.assertEqual([event[1] for event in events if event[0] == "run"], [
            "start", "right", "finish",
        ])
        del pipeline
        gc.collect()
        self.assertTrue(all(ref() is None for ref in references))

    def test_repeated_builds_create_fresh_nodes_and_do_not_register_classes(self):
        events, references = [], []
        node_class = self.make_node_class(events, references)
        registered = GElement.registeredClasses()
        classes = {"node": node_class}
        for _ in range(20):
            pipeline = GPipeline()
            self.assert_ok(pipeline.buildFromJsonStr('{"nodes": [{"name": "node"}]}', classes))
            self.assert_ok(pipeline.process())
            del pipeline
            gc.collect()
            self.assertTrue(all(ref() is None for ref in references))
        self.assertEqual([event for event in events if event[0] == "run"], [("run", "node", 1)] * 20)
        self.assertEqual(GElement.registeredClasses(), registered)

    def test_same_class_map_can_build_independent_pipelines_concurrently(self):
        class LocalNode(GNode):
            def __init__(self):
                super().__init__()
                self.runs = 0

            def run(self):
                self.runs += 1
                return CStatus()

        classes = {"node": LocalNode}

        def execute(_):
            pipeline = GPipeline()
            self.assert_ok(pipeline.buildFromJsonStr('{"nodes": [{"name": "node"}]}', classes))
            self.assert_ok(pipeline.makeSerial())
            self.assert_ok(pipeline.process())

        with ThreadPoolExecutor(max_workers=4) as executor:
            list(executor.map(execute, range(24)))

    def test_invalid_graphs_return_status_before_constructing_nodes(self):
        references = []
        node_class = self.make_node_class([], references)
        cases = [
            ("{", {"node": node_class}, "Invalid graph JSON"),
            ('{"nodes": {}}', {}, "array 'nodes'"),
            ('{"nodes": [{"name": "node"}]}', {}, "node class not found"),
            ('{"nodes": [{"name": "node"}, {"name": "node"}]}', {"node": node_class}, "duplicate node name"),
            ('{"nodes": [{}]}', {}, "node missing name"),
            ('{"nodes": [{"name": "node", "depends": ["absent"]}]}', {"node": node_class}, "unknown node"),
            ('{"nodes": [{"name": "node", "depends": "absent"}]}', {"node": node_class}, "depends must be an array"),
            ('{"nodes": [{"name": "node", "loop": -1}]}', {"node": node_class}, "loop must be"),
            ('{"nodes": [{"name": 123}]}', {}, "Invalid graph JSON"),
        ]
        for graph, classes, message in cases:
            with self.subTest(graph=graph):
                pipeline = GPipeline()
                status = pipeline.buildFromJsonStr(graph, classes)
                self.assertTrue(status.isErr())
                self.assertIn(message, status.getInfo())
        self.assertEqual(references, [])

    def test_invalid_mapping_values_raise_type_error(self):
        pipeline = GPipeline()
        for value in (None, 123, "not a class"):
            with self.subTest(value=value):
                with self.assertRaisesRegex(TypeError, "class or callable"):
                    pipeline.buildFromJsonStr('{"nodes": [{"name": "node"}]}', {"node": value})
        with self.assertRaisesRegex(TypeError, "must construct a GElement"):
            pipeline.buildFromJsonStr('{"nodes": [{"name": "node"}]}', {"node": lambda: object()})

    def test_constructor_exception_keeps_partial_nodes_until_pipeline_cleanup(self):
        references = []
        node_class = self.make_node_class([], references)

        def fail():
            raise ValueError("constructor failed")

        pipeline = GPipeline()
        with self.assertRaisesRegex(ValueError, "constructor failed"):
            pipeline.buildFromJsonStr(
                '{"nodes": [{"name": "first"}, {"name": "second"}]}',
                {"first": node_class, "second": fail},
            )
        self.assertIsNotNone(references[0]())
        del pipeline
        gc.collect()
        self.assertIsNone(references[0]())

    def test_condition_requires_group_class_and_cleans_up_on_error(self):
        references = []
        pipeline = GPipeline()
        status = pipeline.buildFromJsonStr(
            '{"nodes": [{"name": "select", "meta_type": "GCondition"}]}',
            {"select": self.make_node_class([], references)},
        )
        self.assertTrue(status.isErr())
        self.assertIn("not a GGroup", status.getInfo())
        del pipeline
        gc.collect()
        self.assertIsNone(references[0]())

    def test_reused_node_instance_is_rejected(self):
        node = self.make_node_class([])()
        pipeline = GPipeline()
        status = pipeline.buildFromJsonStr(
            '{"nodes": [{"name": "first"}, {"name": "second"}]}',
            {"first": lambda: node, "second": lambda: node},
        )
        self.assertTrue(status.isErr())
        self.assertIn("fresh element", status.getInfo())
        del node

    def test_rejected_foreign_node_is_not_retained_by_the_rejecting_pipeline(self):
        references = []
        node_class = self.make_node_class([], references)
        owner = GPipeline()
        self.assert_ok(owner.buildFromJsonStr('{"nodes": [{"name": "node"}]}', {"node": node_class}))
        other = GPipeline()
        status = other.buildFromJsonStr(
            '{"nodes": [{"name": "node"}]}', {"node": lambda: references[0]()}
        )
        self.assertTrue(status.isErr())
        self.assertIn("fresh element", status.getInfo())
        del owner
        gc.collect()
        self.assertIsNone(references[0]())

    def test_partial_build_node_cannot_be_adopted_by_another_pipeline(self):
        # Isolate this ownership regression: a double adoption would double-delete
        # the native node and crash, not just fail a Python assertion.
        code = textwrap.dedent('''
            import gc
            import weakref
            from pydaograph import CStatus, GNode, GPipeline

            references = []
            class Node(GNode):
                def __init__(self):
                    super().__init__()
                    references.append(weakref.ref(self))
                def run(self):
                    return CStatus()

            def fail():
                raise ValueError("constructor failed")

            owner = GPipeline()
            try:
                owner.buildFromJsonStr(
                    '{"nodes": [{"name": "first"}, {"name": "second"}]}',
                    {"first": Node, "second": fail},
                )
            except ValueError:
                pass
            other = GPipeline()
            result = other.buildFromJsonStr(
                '{"nodes": [{"name": "node"}]}', {"node": lambda: references[0]()}
            )
            assert result.isErr(), "partial-build node was adopted twice"
            assert "fresh element" in result.getInfo()
            del owner
            gc.collect()
            assert references[0]() is None
            del other
        ''')
        result = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True, timeout=20)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_building_initialized_pipeline_is_rejected_before_construction(self):
        references = []
        node_class = self.make_node_class([], references)
        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJsonStr('{"nodes": [{"name": "node"}]}', {"node": node_class}))
        self.assert_ok(pipeline.init())
        try:
            status = pipeline.buildFromJsonStr('{"nodes": [{"name": "extra"}]}', {"extra": node_class})
            self.assertTrue(status.isErr())
            self.assertEqual(len(references), 1)
        finally:
            self.assert_ok(pipeline.destroy())

    def test_file_errors_are_reported_as_status(self):
        pipeline = GPipeline()
        with TemporaryDirectory() as directory:
            path = Path(directory) / "missing.json"
            self.assertTrue(pipeline.buildFromJson(str(path), {}).isErr())
            path.write_text("{", encoding="utf-8")
            status = pipeline.buildFromJson(str(path), {})
            self.assertTrue(status.isErr())
            self.assertIn("Invalid graph JSON", status.getInfo())

    def test_tutorial_graph_runs_with_both_loaders(self):
        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJsonStr(GRAPH_PATH.read_text(encoding="utf-8"), NODE_CLASSES))
        self.assert_ok(pipeline.process())
        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJson(str(GRAPH_PATH), NODE_CLASSES))
        self.assert_ok(pipeline.process())

    def test_async_process_manages_lifecycle_with_both_loaders(self):
        for from_file in (False, True):
            with self.subTest(from_file=from_file):
                events = []
                node_class = self.make_node_class(events)
                classes = {name: node_class for name in NODE_CLASSES}
                pipeline = GPipeline()
                if from_file:
                    status = pipeline.buildFromJson(str(GRAPH_PATH), classes)
                else:
                    status = pipeline.buildFromJsonStr(
                        GRAPH_PATH.read_text(encoding="utf-8"), classes
                    )
                self.assert_ok(status)
                self.assert_ok(pipeline.asyncProcess().get())
                self.assertEqual([event for event in events if event[0] == "run"], [
                    ("run", "start", 1), ("run", "repeat", 1),
                    ("run", "repeat", 2), ("run", "finish", 1),
                ])
                for name in classes:
                    self.assertEqual(events.count(("init", name)), 1)
                    self.assertEqual(events.count(("destroy", name)), 1)

    def test_async_run_uses_explicit_init_and_destroy(self):
        events = []
        pipeline = GPipeline()
        self.assert_ok(pipeline.buildFromJsonStr(
            '{"nodes": [{"name": "node"}]}', {"node": self.make_node_class(events)}
        ))
        self.assert_ok(pipeline.init())
        try:
            self.assert_ok(pipeline.asyncRun().get())
            self.assertEqual(events, [("init", "node"), ("run", "node", 1)])
        finally:
            self.assert_ok(pipeline.destroy())
        self.assertEqual(events.count(("destroy", "node")), 1)

    def test_tutorial_script_runs_with_both_loaders(self):
        result = subprocess.run(
            [sys.executable, str(GRAPH_PATH.with_suffix(".py"))],
            capture_output=True, text=True, timeout=20,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.splitlines(), [
            "Running start", "Running repeat", "Running repeat",
            "finish: all upstream nodes completed",
        ] * 2)


if __name__ == "__main__":
    unittest.main()