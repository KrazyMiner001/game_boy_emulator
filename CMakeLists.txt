cmake_minimum_required(VERSION 3.31)

project(Game_Boy_Emulator VERSION 1.0.0)

set(SOURCES
    src/main.cpp
    src/cpu/memory.cpp
    src/cpu/registers.cpp
    src/cpu/cpu.cpp
)

add_executable(Game_Boy_Emulator ${SOURCES})

target_include_directories(Game_Boy_Emulator
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)