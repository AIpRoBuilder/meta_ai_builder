"""Regression checks for the concurrent JSON request tutorial."""

from concurrent.futures import ThreadPoolExecutor
from dataclasses import FrozenInstanceError
from pathlib import Path
from threading import Barrier, BrokenBarrierError, Event, Lock
import unittest
from unittest.mock import patch

from pydaograph import CStatus, GPipeline
from tutorial.TConcurrentJsonRequests import (
    ConcurrentRequestDoubleNode,
    GRAPH_PATH,
    PARAM_KEY,
    RequestPipelinePool,
    Response,
)


class ConcurrentJsonRequestsTest(unittest.TestCase):
    def test_overlapping_requests_use_distinct_nodes_and_params(self):
        workers = 4
        barrier = Barrier(workers)
        lock = Lock()
        observations = []
        original_run = ConcurrentRequestDoubleNode.run
        original_build = GPipeline.buildFromJson
        builds = []

        def counted_build(pipeline, graph_path):
            builds.append(graph_path)
            return original_build(pipeline, graph_path)

        def overlapping_run(node):
            param = node.getGParam(PARAM_KEY)
            with lock:
                observations.append((id(node), id(param), param.request_id))
            self.assertEqual(param.doubled_values, ())
            self.assertIsNone(param.response)
            try:
                # A request cannot finish this node until all four are inside
                # their own pipelines. This verifies overlap, not just submits.
                barrier.wait(timeout=10)
            except BrokenBarrierError:
                return CStatus(-1, "Concurrent requests did not overlap")
            return original_run(node)

        requests = [
            (f"request-{index}", (index, index + 100))
            for index in range(40)
        ]
        with patch.object(GPipeline, "buildFromJson", counted_build):
            with patch.object(ConcurrentRequestDoubleNode, "run", overlapping_run):
                with RequestPipelinePool(size=workers) as pool:
                    with ThreadPoolExecutor(max_workers=workers * 2) as executor:
                        futures = [
                            executor.submit(pool.handle_request, request_id, values)
                            for request_id, values in requests
                        ]
                        responses = [future.result(timeout=30) for future in futures]
                    self.assertEqual(pool._manager.getSize(), workers)
                    for param in pool._params.values():
                        self.assertEqual(param.request_id, "")
                        self.assertEqual(param.values, ())
                        self.assertEqual(param.doubled_values, ())
                        self.assertIsNone(param.response)

        self.assertEqual(builds, [str(GRAPH_PATH)] * workers)

        for (request_id, values), response in zip(requests, responses):
            doubled = tuple(value * 2 for value in values)
            self.assertEqual(response, Response(request_id, doubled, sum(doubled)))

        # Repeated requests reuse EXACTLY the pool's four nodes/parameters.
        self.assertEqual(len(observations), len(requests))
        self.assertEqual(len({node_id for node_id, _, _ in observations}), workers)
        self.assertEqual(len({param_id for _, param_id, _ in observations}), workers)
        for start in range(0, len(observations), workers):
            generation = observations[start:start + workers]
            self.assertEqual(len({node_id for node_id, _, _ in generation}), workers)
            self.assertEqual(len({param_id for _, param_id, _ in generation}), workers)
            self.assertEqual(len({request_id for _, _, request_id in generation}), workers)

    def test_response_survives_cleanup_and_does_not_reference_input_list(self):
        values = [1, 2, 3]
        with RequestPipelinePool(size=1) as pool:
            response = pool.handle_request("first", values)
            values.append(999)
            self.assertEqual(pool.handle_request("second", [10]), Response("second", (20,), 20))
        self.assertEqual(response, Response("first", (2, 4, 6), 12))
        with self.assertRaises(FrozenInstanceError):
            response.total = 0

    def test_non_integer_input_is_rejected_before_execution(self):
        with RequestPipelinePool(size=1) as pool:
            with self.assertRaisesRegex(TypeError, "must be integers"):
                pool.handle_request("invalid", ["not an integer"])
            self.assertEqual(pool.handle_request("valid", [2]), Response("valid", (4,), 4))

    def test_missing_json_reports_build_error(self):
        with self.assertRaisesRegex(RuntimeError, "buildFromJson failed"):
            RequestPipelinePool(graph_path=Path("missing-request-graph.json"))

    def test_failed_request_clears_and_returns_the_pipeline(self):
        def failing_run(node):
            param = node.getGParam(PARAM_KEY)
            param.doubled_values = (999,)
            param.response = Response("stale", (999,), 999)
            return CStatus(-1, "Injected request failure")

        with RequestPipelinePool(size=1) as pool:
            with patch.object(ConcurrentRequestDoubleNode, "run", failing_run):
                with self.assertRaisesRegex(RuntimeError, "Injected request failure"):
                    pool.handle_request("failure", [99])
            param = next(iter(pool._params.values()))
            self.assertEqual(param.request_id, "")
            self.assertEqual(param.values, ())
            self.assertEqual(param.doubled_values, ())
            self.assertIsNone(param.response)
            self.assertEqual(pool.handle_request("recovered", [3]), Response("recovered", (6,), 6))

    def test_closed_pool_rejects_requests_and_close_is_idempotent(self):
        pool = RequestPipelinePool(size=1)
        pool.close()
        pool.close()
        with self.assertRaisesRegex(RuntimeError, "pool is closed"):
            pool.handle_request("late", [1])

    def test_close_waits_for_active_request_before_destroy(self):
        entered = Event()
        continue_run = Event()
        original_run = ConcurrentRequestDoubleNode.run

        def blocked_run(node):
            entered.set()
            if not continue_run.wait(timeout=10):
                return CStatus(-1, "Test did not release the active request")
            return original_run(node)

        pool = RequestPipelinePool(size=1)
        try:
            with patch.object(ConcurrentRequestDoubleNode, "run", blocked_run):
                with ThreadPoolExecutor(max_workers=2) as executor:
                    request = executor.submit(pool.handle_request, "active", [7])
                    try:
                        self.assertTrue(entered.wait(timeout=10))
                        closing = executor.submit(pool.close)
                        with pool._condition:
                            self.assertTrue(pool._condition.wait_for(lambda: pool._closing, timeout=10))
                        self.assertFalse(closing.done())
                    finally:
                        continue_run.set()
                    self.assertEqual(request.result(timeout=10), Response("active", (14,), 14))
                    closing.result(timeout=10)
        finally:
            continue_run.set()
            pool.close()

    def test_non_positive_pool_size_is_rejected(self):
        for size in (0, -1):
            with self.subTest(size=size):
                with self.assertRaisesRegex(ValueError, "positive integer"):
                    RequestPipelinePool(size=size)


if __name__ == "__main__":
    unittest.main()