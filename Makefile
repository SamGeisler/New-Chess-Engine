INCLUDE= -I C:\mingw_dev_lib\include\SDL2
LIBRARY = -L C:\mingw_dev_lib\lib
COMPILER_FLAGS = 


all:
	gcc $(INCLUDE) $(LIBRARY) $(COMPILER_FLAGS) -o main main.c render.c control.c generate_moves.c bb_utils.c -lmingw32 -lSDL2main -lSDL2

debug:
	gcc $(INCLUDE) $(LIBRARY) -g -o main main.c render.c control.c generate_moves.c bb_utils.c -lmingw32 -lSDL2main -lSDL2

testbox:
	gcc -o test testbox.c -lmingw32
