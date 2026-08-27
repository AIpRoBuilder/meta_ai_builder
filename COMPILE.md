<h1 align="center">
  DaoGraph 编译说明
</h1>

### C++ 版本

* 本工程支持`MacOS`、`Linux`、`Windows`和`Android`系统，无任何第三方依赖。默认使用C++11版本，推荐使用C++17版本，暂不支持C++11以下的版本

* 使用`CLion`(推荐)作为IDE的开发者，打开`CMakeLists.txt`文件作为工程，即可编译通过。本项目已经获得 [Jetbrains 开源开发许可证](https://www.jetbrains.com/zh-cn/community/opensource/#support )，感谢 Jetbrains 提供优秀的IDE，和对项目的认可

* Windows环境中，使用`Visual Studio`(2013版或以上版本)作为IDE的开发者，安装cmake之后，输入以下指令，即可生成`DaoGraph.sln`文件
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder
  $ cmake . -Bbuild    # 在 build 文件夹下，生成对应的 DaoGraph.sln 文件
  ```

* MacOS环境中，使用`Xcode`作为IDE的开发者，安装cmake之后，输入以下指令，即可生成`DaoGraph.xcodeproj`文件
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder
  $ mkdir build && cd build
  $ cmake .. -G Xcode    # 在 build 文件夹下，生成对应的 DaoGraph.xcodeproj 文件
  ```

* Cmake编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder
  $ cmake . -Bbuild
  $ cd build && make -j8
  $ ./tutorial/T00-HelloDaoGraph              # 运行 T00-HelloDaoGraph
  ```

* Bazel编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder
  $ bazel build //tutorial/...              # 编译 tutorial路径下的所有targets
  $ bazel run //tutorial:T00-HelloDaoGraph    # 运行 T00-HelloDaoGraph
  ```

* Xmake编译方式(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder
  $ xmake build                             # 编译 tutorial 中的所有项目
  $ xmake run T00-HelloDaoGraph               # 运行 T00-HelloDaoGraph
  ```

* 提供online版本的编译调试环境，点击进入页面：[DaoGraph env online](https://gitpod.io/#/github.com/AIpRoBuilder/meta_ai_builder) ，通过github账号登录。进入后，输入以下指令，即可编译通过，并查看执行结果
  ```shell
  $ sudo apt-get install cmake -y           # 安装cmake
  $ ./DaoGraph-build.sh                       # 编译DaoGraph工程，生成的内容在同级/build/文件夹中
  $ ./build/tutorial/T00-HelloDaoGraph        # 运行T00-HelloDaoGraph，并且在终端输出 Hello, DaoGraph.
  ```

---

* 在其他使用 CMakeLists.txt 构建的项目中，通过 FetchContent 作为三方库引入
  ```cmake
  set(DAOGRAPH_BUILD_TUTORIALS OFF CACHE BOOL "" FORCE)
  set(DAOGRAPH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(DAOGRAPH_BUILD_FUNCTIONAL_TESTS OFF CACHE BOOL "" FORCE)
  set(DAOGRAPH_BUILD_PERFORMANCE_TESTS OFF CACHE BOOL "" FORCE)

  Include(FetchContent)
  FetchContent_Declare(
    DaoGraph
    GIT_REPOSITORY https://github.com/AIpRoBuilder/meta_ai_builder.git
    GIT_TAG main
    GIT_SHALLOW true
  )

  FetchContent_MakeAvailable(DaoGraph)
  target_include_directories(${PROJECT_NAME} PRIVATE ${DaoGraph_SOURCE_DIR}/src)
  target_link_libraries(${PROJECT_NAME} PRIVATE DaoGraph)
  ```

---

### Python 版本
* 通过`pip`命令安装
  ```shell
  $ python3 -m pip install "git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"
  $ python3 -c "import pydaograph"           # 验证 pydaograph 成功安装
  ```

* 通过源码构建`.whl`安装(Linux/MacOS/Windows)
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder/python                # 进入对应文件夹
  $ python3 -m pip install --upgrade pip build
  $ python3 -m build                         # 生成 *.whl 和 *.tar.gz 安装包
  $ python3 -m pip install dist/pydaograph-*.whl
  $ python3 tutorial/T00-HelloDaoGraph.py    # 运行T00-HelloDaoGraph.py，并且在终端输出 Hello, pydaograph.
  ```

* 克隆仓库后本地安装
  ```shell
  $ git clone https://github.com/AIpRoBuilder/meta_ai_builder.git
  $ cd meta_ai_builder
  $ python3 -m pip install ./python
  $ python3 python/tutorial/T00-HelloDaoGraph.py
  ```

* 通过uv安装(Linux/MacOS/Windows)
  ```shell
  $ uv init daograph_env              # 通过uv 创建一个名为 daograph_env 的环境
  $ cd daograph_env 
  $ uv add "pydaograph @ git+https://github.com/AIpRoBuilder/meta_ai_builder.git@main#subdirectory=python"    # 添加 pydaograph 依赖
  $ python3 -c "import pydaograph"    # 验证 pydaograph 成功安装
  ```

* 通过poetry安装(Linux/MacOS/Windows)
  ```shell
  $ poetry new daograph_env
  $ cd daograph_env
  $ poetry add "git+https://github.com/AIpRoBuilder/meta_ai_builder.git#subdirectory=python"
  $ poetry run python -c "import pydaograph"    # 验证 pydaograph 成功安装
  ```
