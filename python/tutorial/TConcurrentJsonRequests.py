"""One JSON graph, concurrent requests, a reusable GPipelineManager pool.

Build and initialize a fixed number of independent pipelines once. Each request
leases one pipeline exclusively, supplies its inputs, runs it, clears ALL request
data, and returns it to the manager. Excess requests wait for an available slot.

This example requires a synchronous chain and deliberately excludes background
events/daemons and node timeouts. Do not reuse one instance concurrently.
"""

from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
from pathlib import Path
from threading import Condition, local
from typing import Dict, Iterable, List, Optional, Tuple

from pydaograph import CStatus, GElement, GNode, GParam, GPipeline, GPipelineManager


GRAPH_PATH = Path(__file__).with_suffix(".json")
PARAM_KEY = "request"
_build_context = local()


@dataclass(frozen=True)
class Response:
    request_id: str
    doubled_values: Tuple[int, ...]
    total: int


class RequestParam(GParam):
    def __init__(self):
        super().__init__()
        # One instance per pipeline, never class-level mutable state.
        self.request_id = ""
        self.values: Tuple[int, ...] = ()
        self.doubled_values: Tuple[int, ...] = ()
        self.response: Optional[Response] = None

    def clear(self) -> None:
        # Called before release(), on both success and failure.
        self.request_id = ""
        self.values = ()
        self.doubled_values = ()
        self.response = None

    def setup(self) -> CStatus:
        # Do not clear the inputs supplied by handle_request().
        self.doubled_values = ()
        self.response = None
        return CStatus()

    def reset(self, cur_status: CStatus) -> None:
        # run() invokes reset before returning. Clear scratch data, but retain
        # the immutable response so handle_request() can read it afterward.
        self.doubled_values = ()
        if cur_status.isErr():
            self.response = None


class JsonRequestNode(GNode):
    def clone(self):
        # The factory calls creator()->clone(). Only the empty prototype is
        # shared; every executing node is NEW and retained for the pool lifetime.
        node = type(self)()
        _build_context.nodes.append(node)
        return node


def register_request_node(node_type):
    # Register once, outside worker execution. Prototypes are never added to a
    # pipeline or given request data. Caching these factory-only objects avoids
    # allocating leaked prototypes on every build in the current binding.
    prototype = node_type()
    GElement.registerClass(node_type.__name__, lambda: prototype)
    return node_type


@register_request_node
class ConcurrentRequestDoubleNode(JsonRequestNode):
    def run(self) -> CStatus:
        param = self.getGParam(PARAM_KEY)
        if not isinstance(param, RequestParam):
            return CStatus(-1, "Missing request-local RequestParam")
        param.doubled_values = tuple(value * 2 for value in param.values)
        return CStatus()


@register_request_node
class ConcurrentRequestResultNode(JsonRequestNode):
    def run(self) -> CStatus:
        param = self.getGParam(PARAM_KEY)
        if not isinstance(param, RequestParam):
            return CStatus(-1, "Missing request-local RequestParam")
        param.response = Response(
            request_id=param.request_id,
            doubled_values=param.doubled_values,
            total=sum(param.doubled_values),
        )
        return CStatus()


def check_status(status: CStatus, operation: str) -> None:
    if status.isErr():
        raise RuntimeError(f"{operation} failed: {status.getInfo()}")


class RequestPipelinePool:
    """GPipelineManager owns checkout/return; Condition adds waiting/shutdown."""

    def __init__(self, size: int = 4, graph_path: Path = GRAPH_PATH):
        if not isinstance(size, int) or size < 1:
            raise ValueError("Pool size must be a positive integer")

        self._manager = GPipelineManager()
        self._params: Dict[GPipeline, RequestParam] = {}
        self._nodes: List[GNode] = []
        self._condition = Condition()
        self._active_requests = 0
        self._closing = False
        self._closed = False
        self._initialized = False

        try:
            for _ in range(size):
                pipeline = GPipeline()
                param = RequestParam()
                _build_context.nodes = self._nodes
                try:
                    check_status(pipeline.buildFromJson(str(graph_path)), "buildFromJson")
                finally:
                    del _build_context.nodes
                # This sample's chain runs on the requesting thread, with no
                # detached work to drain on errors. Requests still overlap.
                check_status(pipeline.makeSerial(), "makeSerial (synchronous chain required)")
                check_status(pipeline.createGParam(param, PARAM_KEY), "createGParam")
                self._params[pipeline] = param
                # Add each DISTINCT instance once; never add duplicates.
                check_status(self._manager.add(pipeline), "manager.add")

            # Initialize once through the manager, not once per request.
            check_status(self._manager.init(), "manager.init")
            self._initialized = True
        except Exception:
            self.close()
            raise

    def handle_request(self, request_id: str, values: Iterable[int]) -> Response:
        """Lease -> populate -> run -> snapshot -> clear -> release."""
        # Copy incoming containers. Nested mutable payloads need a deep copy
        # or deserialization; the integer payload in this example is immutable.
        input_values = tuple(values)
        if any(not isinstance(value, int) for value in input_values):
            raise TypeError("Request values must be integers")

        with self._condition:
            while True:
                if self._closing or self._closed:
                    raise RuntimeError("Pipeline pool is closed")
                pipeline = self._manager.fetch()
                if pipeline is not None:
                    self._active_requests += 1
                    break
                # fetch() is nonblocking; wait, do not spin or fail on overload.
                self._condition.wait()

        param = self._params[pipeline]
        try:
            param.request_id = request_id
            param.values = input_values
            # Do NOT call manager.run(): it would fetch ANOTHER pipeline before
            # we could attach these inputs and capture this pipeline's response.
            check_status(pipeline.run(), "run")
            response = param.response
            if response is None:
                raise RuntimeError("Pipeline did not produce a response")
            # Response is immutable and detached from the reusable GParam.
            return response
        finally:
            param.clear()
            with self._condition:
                try:
                    check_status(self._manager.release(pipeline), "manager.release")
                finally:
                    self._active_requests -= 1
                    self._condition.notify_all()

    def close(self) -> None:
        """Reject new requests; drain current leases; destroy once via manager."""
        with self._condition:
            while self._closing and not self._closed:
                self._condition.wait()
            if self._closed:
                return
            self._closing = True
            self._condition.notify_all()
            while self._active_requests:
                self._condition.wait()

            try:
                if self._initialized:
                    check_status(self._manager.destroy(), "manager.destroy")
                    self._initialized = False
                check_status(self._manager.clear(), "manager.clear")
            except Exception:
                self._closing = False
                self._condition.notify_all()
                raise

            # Keep Python overrides alive through destroy callbacks, but drop
            # nodelete wrappers BEFORE releasing the C++ pipeline objects.
            self._nodes.clear()
            self._params.clear()
            # The binding's manager.add keep_alive also retains pipelines until
            # the manager wrapper is released, even after manager.clear().
            self._manager = None
            self._closed = True
            self._condition.notify_all()

    def __enter__(self):
        return self

    def __exit__(self, _exc_type, _exc_value, _traceback):
        self.close()


def main() -> None:
    requests = [
        ("request-A", [1, 2, 3]),
        ("request-B", [10, 20]),
        ("request-C", [-2, 5]),
        ("request-D", [100]),
    ]
    # Only two graphs are built. Four request workers lease/reuse them safely.
    with RequestPipelinePool(size=2) as pool:
        with ThreadPoolExecutor(max_workers=4) as executor:
            futures = [
                executor.submit(pool.handle_request, request_id, values)
                for request_id, values in requests
            ]
            for future in futures:
                response = future.result()
                print(
                    f"{response.request_id}: "
                    f"doubled={response.doubled_values}, total={response.total}"
                )


if __name__ == "__main__":
    main()