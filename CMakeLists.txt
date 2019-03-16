cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Ofast -Wall -Wextra -fopenmp -ffast-math")# -funroll-loops")

include_directories(include/glm)
include_directories(include/sdl/include/SDL2)
include_directories(Source)

add_executable(raytracer
        Source/raytrace.cpp
        Source/raytrace.h
        Source/SDLauxiliary.h
        Source/skeleton.cpp
        Source/triangle.cpp
        Source/triangle.h
        Source/utils.cpp
        Source/utils.h
        Source/vectors.h)

find_library(SDL_LIB libsdl2 HINTS include/sdl/lib)
target_link_libraries(raytracer PUBLIC ${SDL_LIB})

#set_target_properties(raytracer PROPERTIES COMPILE_FLAGS "-save-temps")
