import unittest
import base64
from pathlib import Path
from tempfile import TemporaryDirectory

from pydaograph import CStatus, GPipeline
from pydaograph import GNode


class UploadImageNode(GNode):

    def __init__(self, image_path: str, label_path: str):
        super().__init__()
        self._image_ok = False
        self._label_ok = False
        self.ran = False
        self.setWaitForInput(True)
        self.setInputPrompt("[python-test] waiting image+label files ... ")
        self.setInputHandler(self._handle_inputs)

    def _handle_inputs(self, user_inputs) -> CStatus:
        # if len(user_inputs) != 2:
        #     return CStatus(3000, "need 2 input files")

        image_bytes = self._decode_payload(user_inputs[0])
        label_bytes = self._decode_payload(user_inputs[1])

        if not self._is_supported_image(image_bytes):
            return CStatus(3001, "uploaded file is not a supported image")

        if len(label_bytes) == 0:
            return CStatus(3003, "uploaded label file is invalid")

        self._image_ok = True
        self._label_ok = True
        return CStatus()

    def _decode_payload(self, payload: str) -> bytes:
        if payload.startswith(self.BINARY_PREFIX):
            return base64.b64decode(payload[len(self.BINARY_PREFIX):])
        return payload.encode("utf-8")

    @staticmethod
    def _is_supported_image(content: bytes) -> bool:
        if len(content) == 0:
            return False
        if content.startswith(b"\x89PNG\r\n\x1a\n"):
            return True
        if content.startswith(b"\xff\xd8\xff"):
            return True
        if content.startswith(b"P3\n") or content.startswith(b"P6\n"):
            return True
        return False

    def run(self) -> CStatus:
        if not self._image_ok or not self._label_ok:
            return CStatus(3002, "uploaded files were not validated")

        self.ran = True
        return CStatus()

    def clone(self):
        return self


class DownstreamNode(GNode):
    def __init__(self):
        super().__init__()
        self.ran = False

    def run(self) -> CStatus:
        self.ran = True
        return CStatus()

    def clone(self):
        return self

root = Path("/Users/xiechuxi/Desktop/")
image_path = root / "FDE介绍.png"
label_path = root / "工具介绍.png"
pipeline = GPipeline()
upload = UploadImageNode(str(image_path), str(label_path))
downstream = DownstreamNode()

status = pipeline.registerGElement(upload, set(), "upload_node")
status += pipeline.registerGElement(downstream, {upload}, "downstream_node")
status = pipeline.process()