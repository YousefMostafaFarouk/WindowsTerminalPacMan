cmake_minimum_required(VERSION 3.10)
project(WindowsTerminalPacman)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(WindowsTerminalPacman ${SRC_FILES})

target_include_directories(WindowsTerminalPacman PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


