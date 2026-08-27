"""

@File: setup
@Time: 2025/3/6 23:25
@Desc:
"""

import glob
from pathlib import Path
import sysconfig
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import pybind11

__PYDAOGRAPH_NAME__ = "pydaograph"
__PYDAOGRAPH_VERSION__ = "0.3.0"
__PYDAOGRAPH_AUTHOR__ = "peterxie"
__PYDAOGRAPH_AUTHOR_EMAIL__ = "peterxcx@gmail.com"
__PYDAOGRAPH_DESCRIPTION__ = "DaoGraph with python api wrapper by pybind11"
__PYDAOGRAPH_URL__ = "https://github.com/AIpRoBuilder/meta_ai_builder"
__PYDAOGRAPH_LICENSE__ = "Apache License 2.0"
__PYDAOGRAPH_LONG_DESCRIPTION__ = (Path(__file__).resolve().parent / "README.md").read_text(encoding="utf-8")
__PYDAOGRAPH_KEYWORDS__ = [__PYDAOGRAPH_NAME__,
                         "DaoGraph",
                         "dag",
                         "parallel",
                         "airflow",
                         "taskflow",
                         "workflow",
                         "pybind11"]

_sources = ['PyDaoGraph.cpp'] + glob.glob("../src/**/*.cpp", recursive=True)
_extra_compile_args = ["-pthread",
                       "-std=c++11",
                       "-fvisibility=hidden"]
_include_dirs = [pybind11.get_include(),
                 "../src"]


def _iter_python_include_dirs():
    seen = set()
    for key in ("include", "platinclude"):
        include_dir = sysconfig.get_path(key)
        if not include_dir:
            continue

        include_path = Path(include_dir)
        normalized = str(include_path)
        if normalized in seen:
            continue

        seen.add(normalized)
        yield include_path


class BuildExtWithPythonHeadersCheck(build_ext):
    def run(self):
        checked_dirs = []
        for include_dir in _iter_python_include_dirs():
            checked_dirs.append(include_dir)
            if (include_dir / "Python.h").exists():
                return super().run()

        checked_dirs_str = ", ".join(str(path) for path in checked_dirs) or "<unknown>"
        raise RuntimeError(
            "Python development headers were not found. "
            f"Expected Python.h under: {checked_dirs_str}. "
            "Install the matching CPython development package for your interpreter "
            "(for example `python3-devel` on Fedora/RHEL or `python3-dev`/`python3.11-dev` "
            "on Debian/Ubuntu), then retry the build."
        )


_ext_modules = [
    Extension(
        name=f"{__PYDAOGRAPH_NAME__}._core",
        sources=_sources,
        extra_compile_args=_extra_compile_args,
        include_dirs=_include_dirs,
    ),
]

_packages = find_packages(where=".",
                          include=["pydaograph", "pydaograph.*"],
                          exclude=["build*", "dist*", "*.egg-info", "__pycache__"])

setup(
    name=__PYDAOGRAPH_NAME__,
    version=__PYDAOGRAPH_VERSION__,
    author=__PYDAOGRAPH_AUTHOR__,
    author_email=__PYDAOGRAPH_AUTHOR_EMAIL__,
    description=__PYDAOGRAPH_DESCRIPTION__,
    url=__PYDAOGRAPH_URL__,
    license=__PYDAOGRAPH_LICENSE__,
    ext_modules=_ext_modules,
    packages=_packages,
    package_dir={"": "."},
    zip_safe=False,
    cmdclass={"build_ext": BuildExtWithPythonHeadersCheck},
    long_description=__PYDAOGRAPH_LONG_DESCRIPTION__,
    long_description_content_type="text/markdown",
    python_requires=">=3.8",
    keywords=__PYDAOGRAPH_KEYWORDS__,
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
        "Programming Language :: C++",
        "Operating System :: Unix",
        "Operating System :: MacOS :: MacOS X",
        "Operating System :: Microsoft :: Windows",
        "License :: OSI Approved :: Apache Software License",
        "Natural Language :: Chinese (Simplified)",
        "Natural Language :: English",
    ]
)
