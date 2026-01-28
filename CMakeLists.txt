cmake_minimum_required(VERSION 4.2.1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-g")

#For clangd extension
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDL2_LIB_DIR "C:/mingw_dev_lib/lib")
set(SDL2_INCLUDE_DIR "C:/mingw_dev_lib/include/SDL2")

include_directories(${SDL2_INCLUDE_DIR} "include")
link_directories(${SDL2_LIB_DIR})

set(UNIVERSAL_SOURCES
    src/bitboard_ops.cpp
    src/debug_tools.cpp
    src/engine.cpp
    src/execute_move.cpp
    src/game.cpp
    src/generate_moves.cpp
    src/load_precompute.cpp
    src/magic_numbers.cpp
    src/move.cpp
    src/transposition_table.cpp)


add_executable(game ${UNIVERSAL_SOURCES} src/main.cpp src/interface.cpp)
add_executable(movegentest ${UNIVERSAL_SOURCES} src/move_generation_test.cpp)
add_executable(precompute src/precompute.cpp)

set_target_properties(game PROPERTIES LINK_FLAGS "-mconsole")
set_target_properties(precompute PROPERTIES LINK_FLAGS "-mconsole")
set_target_properties(movegentest PROPERTIES LINK_FLAGS "-mconsole")

target_link_libraries(game mingw32 SDL2main SDL2)