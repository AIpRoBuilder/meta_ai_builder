"""
Python package wrapper for CGraph bindings and helper tools.
"""
from importlib import import_module

from . import _core as _core  # load compiled extension
from ._core import *  # re-export extension symbols

# expose tooling as a nested module for convenient imports
pycgraph_tools = import_module(f"{__name__}.pycgraph_tools")
register = pycgraph_tools.register
register_class = pycgraph_tools.register_class

_core_exports = getattr(_core, "__all__", None)
__all__ = list(_core_exports) if _core_exports else [name for name in dir(_core) if not name.startswith("_")]
__all__.extend(["pycgraph_tools", "register", "register_class"])
