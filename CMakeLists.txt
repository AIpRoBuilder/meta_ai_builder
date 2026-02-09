
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")
message("*     ______   ______                             __    *")
message("*    / ____/  / ____/   _____  ____ _    ____    / /_   *")
message("*   / /      / / __    / ___/ / __ `/   / __ \\  / __ \\  *")
message("*  / /___   / /_/ /   / /    / /_/ /   / /_/ / / / / /  *")
message("*  \\____/   \\____/   /_/     \\__,_/   / .___/ /_/ /_/   *")
message("*                                    /_/ by Chunel Feng *")
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")

cmake_minimum_required(VERSION 3.5.0)

project(DaoGraph VERSION 3.2.2)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    # DaoGraph默认使用C++11版本，推荐使用C++17版本。暂不支持C++11以下版本
    set(CMAKE_CXX_STANDARD 11)
endif()

option(DAOGRAPH_BUILD_TUTORIALS "Enable build tutorials" ON)
option(DAOGRAPH_BUILD_EXAMPLES "Enable build examples" ON)
option(DAOGRAPH_BUILD_FUNCTIONAL_TESTS "Enable build functional tests" OFF)
option(DAOGRAPH_BUILD_PERFORMANCE_TESTS "Enable build performance tests" OFF)

# 如果开启此宏定义，则DaoGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_DAOGRAPH_SILENCE_)

# 此宏可以在纯并发的微小任务下，用于提升整体性能。主要用于在性能测试的情况下使用，一般情况不推荐打开
# add_definitions(-D_DAOGRAPH_PARALLEL_MICRO_BATCH_ENABLE_)

# 此宏用于在读写GParam加锁的时候，加锁方式修改为读写锁的情况
# 仅在cpp17或以上版本生效
# 打开后会在多读少写的情况下有性能优化
# 副作用是GParam中的锁无法递归上锁，一般情况下可忽略影响
# add_definitions(-D_DAOGRAPH_GPARAM_RWLOCK_ENABLE_)

# add DaoGraph environment info
include(cmake/DaoGraph-env-include.cmake)

# 教程相关内容
if(DAOGRAPH_BUILD_TUTORIALS)
    message(STATUS "[DaoGraph] build tutorials")
    add_subdirectory(./tutorial)
endif(DAOGRAPH_BUILD_TUTORIALS)

# 样例相关内容
if(DAOGRAPH_BUILD_EXAMPLES)
    message(STATUS "[DaoGraph] build examples")
    add_subdirectory(./example)
endif(DAOGRAPH_BUILD_EXAMPLES)

# 功能测试相关内容
if(DAOGRAPH_BUILD_FUNCTIONAL_TESTS)
    message(STATUS "[DaoGraph] build functional tests")
    add_subdirectory(./test/Functional)
endif(DAOGRAPH_BUILD_FUNCTIONAL_TESTS)

# 性能测试相关内容
if(DAOGRAPH_BUILD_PERFORMANCE_TESTS)
    message(STATUS "[DaoGraph] build performance tests")
    add_subdirectory(./test/Performance)
endif(DAOGRAPH_BUILD_PERFORMANCE_TESTS)
