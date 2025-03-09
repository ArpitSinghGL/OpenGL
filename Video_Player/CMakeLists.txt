cmake_minimum_required(VERSION 3.17)

project(Video_Player)

add_subdirectory(lib/FFmpeg_Video_Decoding)

list(APPEND SOURCES
    src/rendering_grayscale_frames.cpp
    src/load_grayscale_frames.cpp
    lib/glad/glad.c
)

add_executable(Video_Player ${SOURCES})

target_link_libraries(Video_Player FFmpeg_Video_Decoding glfw)