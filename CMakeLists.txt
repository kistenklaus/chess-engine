cmake_minimum_required(VERSION 3.22)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-march=native -g -Wall -O0) #required for x86 instructions

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h" "src/*.hpp" "include/*.h" "include/*.hpp")

add_executable(ChessEngine ${SOURCE_FILES})
