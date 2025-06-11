INCLUDE= -I C:\mingw_dev_lib\include\SDL2
LIBRARY = -L C:\mingw_dev_lib\lib
COMPILER_FLAGS = 


all:
	gcc $(INCLUDE) $(LIBRARY) -o main main.c render.c control.c -lmingw32 -lSDL2main -lSDL2

