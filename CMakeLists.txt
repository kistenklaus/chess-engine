cmake_minimum_required(VERSION 3.8)

set(This chess-engine)

project(${This} C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(HEADERS
        src/Movemap.h
        src/bitmap.h
        src/Board.h
        src/GameState.h
        src/color.h
        src/fen.h
        src/figure.h
        src/move.h
        src/move_generation.h
        src/x86utils.h
        src/pinmask.h
        src/checkmask.h
        src/banmask.h
        src/MagicLookup.h
        src/perfT.h
        src/notation.h
        )

set(MAIN_SOURCE src/main.cpp)

set(SOURCES
        src/fen.cpp
        src/notation.cpp
        src/perfT.cpp
        )


include_directories(src) #kinda sketchy but easier to code.
add_compile_options(
        -W
        -std=c++20
        -march=native        #required for x86 instructions
        -O0
        -fomit-frame-pointer
        -foptimize-sibling-calls
        )
add_executable(${This} ${SOURCES} ${HEADERS} ${MAIN_SOURCE})
#target_precompile_headers(${This} PUBLIC src/Movemap.h)
