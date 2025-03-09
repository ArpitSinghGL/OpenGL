cmake_minimum_required(VERSION 3.17)
project(Triangle_Creation_OpenGL)

list(APPEND SOURCES
    triangle.cpp
    glad/glad.c
)

add_executable(Triangle_Creation_OpenGL ${SOURCES})

target_link_libraries(Triangle_Creation_OpenGL glfw)