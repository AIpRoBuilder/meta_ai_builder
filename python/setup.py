"""

@File: setup
@Time: 2025/3/6 23:25
@Desc:
"""

import glob
from setuptools import setup, Extension, find_packages
import pybind11

__PYDAOGRAPH_NAME__ = "pydaograph"
__PYDAOGRAPH_VERSION__ = "0.1.0"
__PYDAOGRAPH_AUTHOR__ = "peterxie"
__PYDAOGRAPH_AUTHOR_EMAIL__ = "peterxcx@gmail.com"
__PYDAOGRAPH_DESCRIPTION__ = "DaoGraph with python api wrapper by pybind11"
__PYDAOGRAPH_URL__ = "https://github.com/peterxie/DaoGraph"
__PYDAOGRAPH_LICENSE__ = "MIT"
__PYDAOGRAPH_LONG_DESCRIPTION__ = 'You can get everything in DaoGraph github main page : [DaoGraph](https://github.com/peterxie/DaoGraph)'
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
        "License :: OSI Approved :: MIT License",
        "Natural Language :: Chinese (Simplified)",
        "Natural Language :: English",
    ]
)
