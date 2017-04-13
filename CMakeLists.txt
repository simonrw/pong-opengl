cmake_minimum_required(VERSION 3.0)

project(pong)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(include)

add_executable(pong src/main.cpp src/objparser.cpp src/shaders.cpp
    src/glerrors.cpp)

target_link_libraries(pong glew glfw /System/Library/Frameworks/OpenGL.framework)

add_custom_target(run COMMAND ./pong DEPENDS pong)
