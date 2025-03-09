cmake_minimum_required(VERSION 3.17)

project(Triangle_Creation_The_Cherno)

list(APPEND SOURCES
    triangle.cpp
    glad/glad.c
)

add_executable(Triangle_Creation_The_Cherno ${SOURCES})
target_link_libraries(Triangle_Creation_The_Cherno glfw)