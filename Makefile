INCLUDE= -I C:\mingw_dev_lib\include\SDL2
LIBRARY = -L C:\mingw_dev_lib\lib
COMPILER_FLAGS = 


all:
	gcc $(INCLUDE) $(LIBRARY) $(COMPILER_FLAGS) -o main main.c render.c control.c generate_moves.c board_utils.c load_precompute.c player_input.c -lmingw32 -lSDL2main -lSDL2
debug:
	gcc $(INCLUDE) $(LIBRARY) -g -o main main.c render.c control.c generate_moves.c board_utils.c load_precompute.c player_input.c -lmingw32 -lSDL2main -lSDL2

perft:
	gcc $(COMPILER_FLAGS) -g -O0 -o perft perft.c generate_moves.c control.c board_utils.c load_precompute.c -lmingw32
perftNDB:
	gcc $(COMPILER_FLAGS) -o  perft perft.c generate_moves.c control.c board_utils.c load_precompute.c -lmingw32

precompute:
	g++ $(COMPILER_FLAGS) -g -O0 -fno-inline -std=c++17 -o precompute precompute.cpp -lmingw32
precomputeNDB:
	g++ $(COMPILER_FLAGS) -o precompute precompute.cpp -lmingw32
