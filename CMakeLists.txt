cmake_minimum_required(VERSION 3.22)
project(chess-engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-march=native -g -Wall -O0 -std=c++17) #required for x86 instructions

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h" "src/*.hpp" "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
