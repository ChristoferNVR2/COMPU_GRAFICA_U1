cmake_minimum_required(VERSION 3.30)
project(legacy_sample)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(legacy_sample main.cpp GLMatrix.h)

# Link both OpenGL, GLU, and GLUT
target_link_libraries(legacy_sample PRIVATE
        OpenGL::GL
        OpenGL::GLU     # Add this line
        GLUT::GLUT
)

# Enable warnings
if(MSVC)
    target_compile_options(legacy_sample PRIVATE /W4)
else()
    target_compile_options(legacy_sample PRIVATE -Wall -Wextra -pedantic)
endif()