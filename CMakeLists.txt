cmake_minimum_required(VERSION 4.2.1)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#For clangd extension
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDL2_LIB_DIR "C:/mingw_dev_lib/lib")
set(SDL2_INCLUDE_DIR "C:/mingw_dev_lib/lib")
set(SDL2_LIBRARY "C:/mingw_dev_lib/bin")

find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUE_DIRS} "include")

set(UNIVERSAL_SOURCES
    src/bitboard_ops.cpp
    src/debug_tools.cpp
    src/engine.cpp
    src/execute_move.cpp
    src/game.cpp
    src/generate_moves.cpp
    src/interface.cpp
    src/load_precompute.cpp
    src/magic_numbers.cpp
    src/move.cpp
    src/transposition_table.cpp)

add_executable(${PROJECT_NAME} ${UNIVERSAL_SOURCES} src/main.cpp)
add_executable(Precompute ${UNIVERSAL_SOURCES} src/precompute.cpp)
add_executable(MoveGenTest ${UNIVERSAL_SOURCES} src/move_generation_test.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})