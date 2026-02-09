"""
Python package wrapper for DaoGraph bindings and helper tools.
"""
from importlib import import_module

from . import _core as _core  # load compiled extension
from ._core import *  # re-export extension symbols

# expose tooling as a nested module for convenient imports
pydaograph_tools = import_module(f"{__name__}.pydaograph_tools")
register = pydaograph_tools.register
register_class = pydaograph_tools.register_class

_core_exports = getattr(_core, "__all__", None)
__all__ = list(_core_exports) if _core_exports else [name for name in dir(_core) if not name.startswith("_")]
__all__.extend(["pydaograph_tools", "register", "register_class"])
