cmake_minimum_required(VERSION 3.10)

project(AutoEdgeSearch VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

add_executable(AutoEdgeSearch src/main.cpp)  # 创建一个可执行文件目标 MyExecutable
