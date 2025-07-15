INCLUDE= -I C:\mingw_dev_lib\include\SDL2
LIBRARY = -L C:\mingw_dev_lib\lib
COMPILER_FLAGS = -std=c++17 


all:
	g++ $(INCLUDE) $(LIBRARY) $(COMPILER_FLAGS) -o main main.cpp interface.cpp game.cpp move.cpp execute_move.cpp bitboard_ops.cpp debug_tools.cpp generate_moves.cpp load_precompute.cpp magic_numbers.cpp -lmingw32 -lSDL2main -lSDL2
debug:
	g++ $(INCLUDE) $(LIBRARY) $(COMPILER_FLAGS) -g -o main main.cpp interface.cpp game.cpp move.cpp execute_move.cpp bitboard_ops.cpp debug_tools.cpp generate_moves.cpp load_precompute.cpp magic_numbers.cpp -lmingw32 -lSDL2main -lSDL2

MGtest:
	g++ $(COMPILER_FLAGS) -g -O0 -o MGtest move_generation_test.cpp game.cpp generate_moves.cpp bitboard_ops.cpp debug_tools.cpp magic_numbers.cpp load_precompute.cpp execute_move.cpp move.cpp -lmingw32
MGtestNDB:
	g++ $(COMPILER_FLAGS) -o MGtest move_generation_test.cpp game.cpp generate_moves.cpp bitboard_ops.cpp debug_tools.cpp magic_numbers.cpp load_precompute.cpp execute_move.cpp move.cpp -lmingw32

precompute:
	g++ $(COMPILER_FLAGS) -g -O0 -fno-inline -std=c++17 -o precompute precompute.cpp -lmingw32
precomputeNDB:
	g++ $(COMPILER_FLAGS) -o precompute precompute.cpp -lmingw32
