cmake_minimum_required(VERSION 3.10)

# dont allow building in root
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(chess LANGUAGES C ASM)

add_executable(chess
    src/generator.s
    src/main.c
    src/input.c
    src/chess.c
    src/engine.c
    src/hashmap.c
)
target_include_directories(chess PRIVATE include/)
