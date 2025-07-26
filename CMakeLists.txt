cmake_minimum_required(VERSION 3.10)

# dont allow building in root
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(chess)

add_executable(chess src/main.c src/terminal.c src/chess.c)
target_include_directories(chess PRIVATE include/)
