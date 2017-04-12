cmake_minimum_required(VERSION 3.0)

project(pong)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable(pong src/main.cpp)

target_link_libraries(pong glew glfw /System/Library/Frameworks/OpenGL.framework)
