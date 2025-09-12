cmake_minimum_required(VERSION 3.12)
project(MyApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -Wpedantic -Werror -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

file(GLOB SOURCES
    "src/*.cpp"
    "src/**/*.cpp"   # Requires CMake ≥ 3.12 for GLOB_RECURSE with globstar
)


add_executable(GpuStreamProcessing main.cpp ${SOURCES})
