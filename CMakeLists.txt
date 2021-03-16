# cmake_minimum_required(VERSION <specify CMake version here>)
project(ParallelMandelbrot)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)

add_executable(ParallelMandelbrot main.cpp tga.cpp tga.h mandelbrot.h mandelbrot.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(ParallelMandelbrot PUBLIC OpenMP::OpenMP_CXX)
endif()