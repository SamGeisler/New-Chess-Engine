INCLUDE= -I C:\mingwDevLib\include\SDL2
LIBRARY = -L C:\mingwDevLib\lib
COMPILER_FLAGS = -std=c++17 


all:
	g++ $(INCLUDE) $(LIBRARY) $(COMPILER_FLAGS) -o main main.cpp render.cpp control.cpp generateMoves.cpp boardUtils.cpp loadPrecompute.cpp playerInput.cpp -lmingw32 -lSDL2main -lSDL2
debug:
	g++ $(INCLUDE) $(LIBRARY) $(COMPILER_FLAGS) -g -o main main.cpp render.cpp control.cpp generateMoves.cpp boardUtils.cpp loadPrecompute.cpp playerInput.cpp -lmingw32 -lSDL2main -lSDL2

perft:
	g++ $(COMPILER_FLAGS) -g -O0 -o perft perft.cpp generateMoves.cpp control.cpp boardUtils.cpp loadPrecompute.cpp -lmingw32
perftNDB:
	g++ $(COMPILER_FLAGS) -o  perft perft.cpp generateMoves.cpp control.cpp boardUtils.cpp loadPrecompute.cpp -lmingw32

precompute:
	g++ $(COMPILER_FLAGS) -g -O0 -fno-inline -std=c++17 -o precompute precompute.cpp -lmingw32
precomputeNDB:
	g++ $(COMPILER_FLAGS) -o precompute precompute.cpp -lmingw32
