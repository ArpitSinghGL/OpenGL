cmake_minimum_required(VERSION 3.17)
project(FFmpeg_Video_Decoding VERSION 0.1.0 LANGUAGES C CXX)

# Find FFmpeg Libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil libswresample libswscale)

# Create an imported interface library
add_library(FFmpeg_Video_Decoding INTERFACE IMPORTED GLOBAL)

# Set FFmpeg include directories and linked libraries as interface properties
set_target_properties(FFmpeg_Video_Decoding PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${FFMPEG_LIBRARIES}"
)