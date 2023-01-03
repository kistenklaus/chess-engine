cmake_minimum_required(VERSION 3.8)

set(This chess-engine)

project(${This} C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_STANDARD 99)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_COMPILER "gcc")

# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(BUILD_SHARED_LIBS ON)

add_compile_options(-march=native -g -Wall -O0 -std=c++20) #required for x86 instructions

enable_testing()

add_subdirectory(googletest)

file(GLOB_RECURSE HEADERS 
  "src/*.h"
  "src/*.hpp"
  "include/*.h"
  "include/*.hpp"
)

file(GLOB SOURCES
  "src/*.cpp",
  "src/*.c"
)



add_library(${This}.lib STATIC ${SOURCES} ${HEADERS})

add_subdirectory(test)


file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h" "src/*.hpp" "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
