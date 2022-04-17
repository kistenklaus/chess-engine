cmake_minimum_required(VERSION 3.22)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h" "src/*.hpp" "include/*.h" "include/*.hpp")

add_executable(ChessEngine ${SOURCE_FILES})
