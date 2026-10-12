from pathlib import Path

from pydaograph import CStatus, GPipeline
from pydaograph import GNode


class UploadImageNode(GNode):

    def __init__(self, image_path: str, label_path: str):
        super().__init__()
        self._image_path = Path(image_path)
        self._label_path = Path(label_path)
        self.ran = False
        self.setWaitFunc(self._files_uploaded)

    def _files_uploaded(self) -> bool:
        return self._image_path.is_file() and self._label_path.is_file()

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
        if not self._is_supported_image(self._image_path.read_bytes()):
            return CStatus(3001, "uploaded file is not a supported image")

        if len(self._label_path.read_bytes()) == 0:
            return CStatus(3003, "uploaded label file is invalid")

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