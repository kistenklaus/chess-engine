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
        src/x86utils.h
        src/pinmask.h
        src/checkmask.h
        src/banmask.h
        src/MagicLookup.h
        src/perfT.h
        src/notation.h
        src/movegen_shift.h
        src/movegen_util.h
        src/movegen_movestack.h
        src/movegen_constant_enumeration.h
        src/movegen_const_collection.h
        src/ChessPosition.h
        src/cli.h
        )

set(MAIN_SOURCE src/main.cpp)

set(SOURCES
        src/fen.cpp
        src/notation.cpp
        src/perfT.cpp
        src/cli.cpp
        src/ChessPosition.cpp
        src/movegen_collection.h)


include_directories(src) #kinda sketchy but easier to code.
add_compile_options(
        -std=c++20
        -march=native        #required for x86 instructions
        -O0
        -fomit-frame-pointer
        -foptimize-sibling-calls
)
add_executable(${This} ${SOURCES} ${HEADERS} ${MAIN_SOURCE})
#target_precompile_headers(${This} PUBLIC src/Movemap.h)
