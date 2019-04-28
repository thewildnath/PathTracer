cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast -Wall -Wextra -fopenmp -ffast-math")# -funroll-loops")

include_directories(include/sdl/include/SDL2)
include_directories(Source)

add_executable(raytracer
        Source/raytrace.cpp
        Source/raytrace.h
        Source/SDLauxiliary.h
        Source/main.cpp
        Source/triangle.cpp
        Source/triangle.h
        Source/utils.cpp
        Source/utils.h
        Source/vector_type.h Source/math_utils.h Source/ray.h Source/camera.h Source/object.h Source/intersection.h Source/material.h Source/geometry.h Source/scene.h Source/light.h)

find_library(SDL_LIB libsdl2 HINTS include/sdl/lib)
target_link_libraries(raytracer PUBLIC ${SDL_LIB})

