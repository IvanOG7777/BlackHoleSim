cmake_minimum_required(VERSION 3.28)
project(ParticleSim)

set(CMAKE_CXX_STANDARD 20)

add_executable(BlackHoleSim
        Source/main.cpp
        Source/glad.c
        Header/Core.h
        Header/Particle.cpp
        Header/Particle.h
)

target_include_directories(BlackHoleSim PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/include
)

target_link_libraries(BlackHoleSim PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/lib/glfw3.lib
        opengl32
)