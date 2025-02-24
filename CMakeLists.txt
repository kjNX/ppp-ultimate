cmake_minimum_required(VERSION 3.30)
project(NewRayTracer)

set(CMAKE_CXX_STANDARD 26)

add_executable(NewRayTracer main.cpp
        Vec.hpp
        Sphere.hpp)
