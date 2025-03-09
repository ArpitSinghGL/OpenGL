cmake_minimum_required(VERSION 3.17)

project(Window_Creation_OpenGL)

list(APPEND SOURCES
    window.cpp
    glad/glad.c
)

add_executable(Window_Creation_OpenGL ${SOURCES})
target_link_libraries(Window_Creation_OpenGL glfw)