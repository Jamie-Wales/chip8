cmake_minimum_required(VERSION 3.14)
project(chip8_emulator C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_library(RAYLIB_LIB NAMES raylib PATHS /usr/local/lib /opt/homebrew/lib /usr/lib /lib)
find_path(RAYLIB_INCLUDE_DIR NAMES raylib.h PATHS /usr/local/include /opt/homebrew/include /usr/include)
if(NOT RAYLIB_LIB)
    message(FATAL_ERROR "Raylib library not found")
endif()
if(NOT RAYLIB_INCLUDE_DIR)
    message(FATAL_ERROR "Raylib include directory not found")
endif()
file(GLOB_RECURSE CHIP8_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.c"
)
file(GLOB_RECURSE CHIP8_HEADERS
    "${PROJECT_SOURCE_DIR}/include/*.h"
)
add_executable(chip8
    ${CHIP8_SOURCES}
)
target_include_directories(chip8 PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${RAYLIB_INCLUDE_DIR}
)

target_link_libraries(chip8 PRIVATE ${RAYLIB_LIB})  # Changed from RAYLIB_LIBRARIES to RAYLIB_LIB
target_compile_options(chip8 PRIVATE -Wall -Wextra -Wpedantic)
