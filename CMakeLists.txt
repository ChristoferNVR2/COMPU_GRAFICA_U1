cmake_minimum_required(VERSION 3.30)
project(legacy_sample)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(legacy_sample main.cpp
        GLMatrix.h)

target_link_libraries(legacy_sample PRIVATE OpenGL::GL GLUT::GLUT)
