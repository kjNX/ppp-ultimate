cmake_minimum_required(VERSION 3.30)
project(ppp-ultimate)

set(CMAKE_CXX_STANDARD 26)

add_executable(NewRayTracer main.cpp
        Vec.hpp
        Sphere.hpp
        Material.hpp
        Light.hpp
        Commons.hpp
        Commons.cpp
        Render.cpp
        Render.hpp
        #Model.cpp
        #Model.hpp
)
