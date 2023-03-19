cmake_minimum_required(VERSION 3.8)

set(This chess-engine)

project(${This} C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(HEADERS
        "src/bitmap.h"
        "src/Board.h"
        "src/BoardState.h"
        "src/build_config.h"
        "src/color.h"
        "src/display.h"
        "src/EastSlidingLookUpTable.h"
        "src/fen.h"
        "src/figure.h"
        "src/KingLookUpTable.h"
        "src/KnightLookUpTable.h"
        "src/lookup.h"
        "src/move.h"
        "src/move_generation.h"
        "src/NorthEastSlidingLookUpTable.h"
        "src/NorthSlidingLookUpTable.h"
        "src/NorthWestSlidingLookUpTable.h"
        "src/SouthEastSlidingLookUpTable.h"
        "src/SouthSlidingLookUpTable.h"
        "src/SouthWestSlidingLookUpTable.h"
        "src/tile_index.h"
        "src/WestSlidingLookUpTable.h"
        "src/x86utils.h"
        "src/checkmask_generation.h"
        "src/pinmask_generation.h"
        "src/banmask_generation.h"
        "src/pinmask.h"
        "src/checkmask.h"
        "src/banmask.h"
        "src/BoardCompiletimeState.h"
        "src/Movestack.h"
        "src/MoveCounter.h"
        "src/MoveBitmapAccumulator.h"
        "src/MoveAccumulator.h"
        )

set(SOURCES
        "src/bitmap.cpp"
        "src/Board.cpp"
        "src/display.cpp"
        "src/EastSlidingLookUpTable.cpp"
        "src/fen.cpp"
        "src/figure.cpp"
        "src/KingLookUpTable.cpp"
        "src/KnightLookUpTable.cpp"
        "src/NorthEastSlidingLookUpTable.cpp"
        "src/NorthSlidingLookUpTable.cpp"
        "src/NorthWestSlidingLookUpTable.cpp"
        "src/SouthEastSlidingLookUpTable.cpp"
        "src/SouthSlidingLookUpTable.cpp"
        "src/SouthWestSlidingLookUpTable.cpp"
        "src/WestSlidingLookUpTable.cpp"
        )

set(MAIN_SOURCE "src/main.cpp")

include_directories(src) #kinda sketchy but easier to code.
add_compile_options(-march=native -g -Wall -O0 -std=c++20) #required for x86 instructions
add_executable(${This} ${SOURCES} ${HEADERS} ${MAIN_SOURCE})


#==================== TESTING ================

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCES
        "test/movegen_common_basic.cpp"
        "test/movegen_bishop_basic.cpp"
        "test/pinmaskgen_rook.cpp"
        "test/movegen_knight_basic.cpp"
        "test/movegen_rook_basic.cpp"
        "test/movegen_pawn_basic.cpp"
        "test/movegen_king_basic.cpp"
        "test/movegen_queen_basic.cpp"
        "test/pinmaskgen_bishop.cpp"
        "test/pinmaskgen_queen.cpp"
        "test/checkmaskgen_pawn.cpp"
        "test/checkmaskgen_knight.cpp"
        "test/checkmaskgen_bishop.cpp"
        "test/checkmaskgen_rook.cpp"
        test/checkmaskgen_queen.cpp)

set(TEST_HEADERS
        "test/MoveTestReceiver.h"
        "test/test_utils.h"
        test/banmaskgen_pawn.cpp test/banmaskgen_king.cpp test/banmaskgen_knight.cpp test/banmaskgen_bishop.cpp test/banmaskgen_rook.cpp test/banmaskgen_queen.cpp test/movegen_king_banmask.cpp test/movegen_pawn_checkmask.cpp test/movegen_knight_checkmask.cpp test/movegen_bishop_checkmask.cpp test/movegen_rook_checkmask.cpp test/movegen_queen_checkmask.cpp test/movegen_knight_pinmask.cpp test/movegen_rook_pinmask.cpp test/movegen_bishop_pinmask.cpp)


add_executable(${This}.test
        ${TEST_SOURCES}
        ${TEST_HEADERS}
        ${HEADERS}
        ${SOURCES}
        )
target_link_libraries(${This}.test
        GTest::gtest_main
        )

include(GoogleTest)
gtest_discover_tests(${This}.test)
