cmake_minimum_required(VERSION 3.28)
project(ParticleSim)

set(CMAKE_CXX_STANDARD 20)

add_executable(BlackHoleSim
        Source/glad.c
        Source/main.cpp
        Source/Particle.cpp

        Header/Particle.h
        Header/Constants.h
        Header/Physics.h
        Source/Physics.cpp
        Header/Input.h
        Source/Input.cpp
        Header/Blackhole.h
        Source/Blackhole.cpp
        Header/Geometry.h
        Source/Geometry.cpp
        Source/GLutils.cpp
        Header/GLutils.h
        Header/Sphere.h
        Header/Camera.h
        Source/Sphere.cpp
)

target_include_directories(BlackHoleSim PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/include
)

target_link_libraries(BlackHoleSim PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/lib/glfw3.lib
        opengl32
)