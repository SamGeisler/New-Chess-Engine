INCLUDE = -IC:\mingw_dev_lib\include\SDL2 -Iinclude
LIBRARY = -LC:\mingw_dev_lib\lib
FLAGS = -std=c++17 -MMD -MP
DEBUG_FLAGS = -g -O0 -fno-inline

MAIN_SRC = main.cpp interface.cpp game.cpp move.cpp execute_move.cpp bitboard_ops.cpp debug_tools.cpp generate_moves.cpp load_precompute.cpp magic_numbers.cpp
MAIN_OBJ = $(addprefix obj/, $(MAIN_SRC:.cpp=.o))

MGTEST_SRC = move_generation_test.cpp game.cpp move.cpp execute_move.cpp bitboard_ops.cpp debug_tools.cpp generate_moves.cpp load_precompute.cpp magic_numbers.cpp
MGTEST_OBJ = $(addprefix obj/,$(MGTEST_SRC:.cpp=.o))

DEPS = $(MAIN_OBJ:.o=.d) $(MGTEST_OBJ:.o=.d)

main: $(MAIN_OBJ)
	g++ $(INCLUDE) $(LIBRARY) $(FLAGS) $^ -o $@ -lmingw32 -lSDL2main -lSDL2

-include $(DEPS)

debug: $(MAIN_OBJ)
	g++ $(INCLUDE) $(LIBRARY) $(FLAGS) $(DEBUG_FLAGS) $^ -o main -lmingw32 -lSDL2main -lSDL2

MGtest: $(MGTEST_OBJ)
	g++ $(INCLUDE) $(LIBRARY) $(FLAGS) $^ -o $@ -lmingw32

MGtestDB: $(MGTEST_OBJ)
	g++ $(INCLUDE) $(LIBRARY) $(FLAGS) $(DEBUG_FLAGS) $^ -o MGtest -lmingw32

precompute:
	g++ $(FLAGS) $(DEBUG_FLAGS) -std=c++17 -o precompute precompute.cpp -lmingw32

obj/%.o: src/%.cpp | obj
	g++ -c $(FLAGS) $(INCLUDE) $< -o $@

obj:
	mkdir obj

clean:
	rmdir obj /S /Q
	del *.exe /Q

