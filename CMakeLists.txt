cmake_minimum_required(VERSION 3.12)
project(MyApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES
    "src/*.cpp"
    "src/**/*.cpp"   # Requires CMake ≥ 3.12 for GLOB_RECURSE with globstar
)


add_executable(GpuStreamProcessing main.cpp ${SOURCES})
