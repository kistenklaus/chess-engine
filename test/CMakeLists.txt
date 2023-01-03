cmake_minimum_required(VERSION 3.8)

set(This chess-engine-tests)

file(GLOB_RECURSE HEADERS 
  "src/*.h",
  "src/*.hpp",
  "include/*.h",
  "include/*.hpp"
)
  

file(GLOB_RECURSE SOURCES
  "src/*.cpp",
  "src/*.c"
)

add_executable(${This} ${SOURCES} ${HEADERS})

target_link_libraries(${This} PUBLIC 
  gtest_main
  chess-engine.lib
)

add_test(
  NAME ${This}
  COMMAND ${This}
)


